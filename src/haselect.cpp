#include <Arduino.h>
#include <PubSubClient.h>
#include <haselect.h>

const char *HASelect::component PROGMEM  = "select";

HASelect::HASelect(const char *unique_id, const char *name, u_int8_t max_options):
    HAEntity(unique_id,name,NULL,component)
{
    this->maxOptions = max_options;
    this->optionsAdded = 0;
    this->state = NULL;
    this->options = new char*[max_options];
}

HASelect::HASelect(const char *unique_id,const char *name, u_int8_t max_options,
    const char *options[]) : HAEntity(unique_id,name,NULL,component) {
    this->maxOptions = max_options;
    this->state = NULL;
    this->options = (char **) options;
    this->optionsAdded = max_options;
}



HASelect::HASelect(const char *unique_id, const char *name, HADevice& device,
    u_int8_t max_options):HASelect(unique_id,name,max_options) {
        this->device = &device;
}

HASelect::HASelect(const char *unique_id, const char *name,
    HADevice& device, u_int8_t max_options, const char* options[]):
    HASelect(unique_id,name,max_options,options) {
        this->device = &device;
}


void HASelect::onConnect(PubSubClient * client) {
    char topic[HA_MAX_TOPIC_LENGTH],payload[HA_MAX_PAYLOAD_LENGTH];
    getCommandTopic(topic);
    client->subscribe(topic);

    this->getConfigTopic(topic);
    this->getConfigPayload(payload,true,true);
    // Add options
    int index = strlen(payload) - 1;
    if (this->optionsAdded > 0)
    {
        sprintf(payload+index,",\"options\":[");
        for (int i=0;i<this->optionsAdded;i++)
        {
            index = strlen(payload);
            sprintf(payload+index,"\"%s\",",this->options[i]);
        }
        index = strlen(payload) - 1;
        sprintf(payload+index,"]}");
    }
    client->publish(topic,payload);
}

void HASelect::addOption(const char *option) {
    if(this->optionsAdded >= this->maxOptions)
        return;
    this->options[this->optionsAdded] = new char[strlen(option)+1];
    strcpy(this->options[this->optionsAdded],option);
    this->optionsAdded++;
}

void HASelect::sendState(PubSubClient * client) {
    char topic[HA_MAX_TOPIC_LENGTH];
    if (state == NULL)
        return;
    getStateTopic(topic);
    client->publish(topic,state);
    dirty = false;
}

void HASelect::setState(const char *txt) {
    for (int i=0;i<this->optionsAdded;i++)
    {
        if(strcmp(this->options[i],txt) == 0)
        {
            this->state = this->options[i];
            dirty = true;
            this->onStateChange();
            return;
        }
    }
}


void HASelect::onReceivedTopic(PubSubClient * client, byte *payload,
    unsigned int length)
{
    for (int i=0;i<this->optionsAdded;i++)
    {
        if(strncmp(this->options[i],(char *)payload,length) == 0)
        {
            this->state = this->options[i];
            dirty = true;
            this->onStateChange();
            return;
        }
    }
}

