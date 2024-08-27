#include"hamqttcontroller.h"
#include"hadevice.h"

// Delay the first state send, HA need some time to process MQTT discovery
// and enable the entities
#define DELAY_SEND_STATES 3000

const char *HAMQTTController::topicHass PROGMEM = "homeassistant/status";

HAMQTTController *HAMQTTController::instance = NULL;

void HAMQTTController::pubSubClientHandler(char* topic, byte* payload,
    unsigned int length)
{
    instance->mqttOnReceived(topic,payload,length);
}

HAMQTTController& HAMQTTController::getInstance() {
    if (instance == NULL)
        instance = new HAMQTTController();
    return *instance;
}

HAMQTTController::HAMQTTController() {
    this->mqttClient = NULL;
    this->entities = NULL;
    this->entityCounter = 0;
    this->callback = NULL;
    this->state = Disconnected;
    this->lastWillDevice = NULL;
}

void HAMQTTController::begin(PubSubClient& mqtt_client,int component_count) {
    this->mqttClient = &mqtt_client;
    this->entities = new HAEntity*[component_count];
    this->entityCounter = 0;
    this->mqttClient->setCallback(HAMQTTController::pubSubClientHandler);
    if (this->mqttClient->getBufferSize() < HA_MAX_PAYLOAD_LENGTH)
        this->mqttClient->setBufferSize(HA_MAX_PAYLOAD_LENGTH);
}

void HAMQTTController::addEntity(HAEntity& entity) {
    this->entities[this->entityCounter] = &entity;
    this->entityCounter++;
    if (this->lastWillDevice == NULL && entity.getDevice() != NULL)
        this->lastWillDevice = entity.getDevice();
}

boolean HAMQTTController::connect(const char *id, const char *user,
    const char *pass) {

    if (this->mqttClient == NULL)
        return false;
    if (this->lastWillDevice != NULL)
    {
        char will_topic[HA_MAX_TOPIC_LENGTH];
        this->lastWillDevice->getAvailabilityTopic(will_topic);
        if (!this->mqttClient->connect(id,user,pass,will_topic,0,false,"offline"))
            return false;
    } else if (!this->mqttClient->connect(id,user,pass))
        return false;
    this->onConnect();
    return this->mqttClient->connected();

}

void HAMQTTController::onConnect() {
    this->mqttClient->subscribe(topicHass);
    HAEntity *entity;
    for (int i = 0; i < this->entityCounter; i++){
        entity = this->entities[i];
        entity->onConnect(this->mqttClient);
    }
}

void HAMQTTController::sendAllStates() {
    HAEntity *entity;
    HADevice *device;
    for (int i = 0; i < this->entityCounter; i++) {
        entity = this->entities[i];
        entity->sendState(this->mqttClient);
        entity->sendAvailable(this->mqttClient,true);
        device = entity->getDevice();
        if (device != NULL)
            device->sendAvailable(this->mqttClient,true);
    }
}

bool HAMQTTController::mqttOnReceived(char *topic, byte *payload,
    unsigned int length) {

    char cmd_topic_buffer[HA_MAX_TOPIC_LENGTH];

    if (strcmp(topic,topicHass) == 0 && length > 3 && payload[1] == 'n') {
        // HA topic is received then HA has been restarted and data is required
        this->onConnect();
        return false;
    }
    else for (int i = 0; i < this->entityCounter; i++){
        const char *cmd_topic = this->entities[i]->getCommandTopic(cmd_topic_buffer);
        HAEntity *entity = this->entities[i];
        if (cmd_topic != NULL &&  strcmp(cmd_topic,topic) == 0)
        {
            entity->onReceivedTopic(this->mqttClient,payload,length);
            if(this->callback != NULL)
                this->callback(entity,topic,payload,length);
            entity->sendState(this->mqttClient);
            return true;
        }
    }
    if(this->callback != NULL)
        this->callback(NULL,topic,payload,length);
    return false;
}

void HAMQTTController::setCallback(HAMQTT_CALLBACK_SIGNATURE(callback)) {
    this->callback = callback;
}

void HAMQTTController::loop() {
    HAEntity *entity;

    if (mqttClient == NULL)
        return;

    if (!mqttClient->connected()) {
        this->state = Disconnected;
        return;
    }

    if(this->state == Disconnected && this->mqttClient->connected()) {
        this->onConnect();
        // Delay the first state send, HA need some time to process MQTT discovery
        delaySendState = millis();
        this->state = Connecting;
    }
    else if(state == Connecting)
    {
        if (millis() - delaySendState  > DELAY_SEND_STATES )
        {
            this->sendAllStates();
            delaySendState = millis() - (1000L*3600L*24L);
            state = Connected;
        }
    }
    else if(state == Connected)
    {
        for (int i = 0 ; i < entityCounter ; i++)
            {
                entity = entities[i];
                if (entity->isDirty())
                    entity->sendState(mqttClient);
                entity->sendAvailable(mqttClient);
                if (!mqttClient->connected())
                    break;

            }
    }
    state = mqttClient->loop()?state:Disconnected;
}

void HAMQTTController::setAvailable(bool available, HADevice& device) {
    HAEntity *entity;
    for (int i = 0; i < this->entityCounter; i++){
        entity = this->entities[i];
        if (entity->getDevice() == &device)
            entity->setAvailable(available);
    }
}