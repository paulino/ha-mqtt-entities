#ifndef __HASELECT_H__
#define __HASELECT_H__

#include "haentity.h"

class PubSubClient;
class HADevice;

class HASelect : public HAEntity {
    protected:

        static const char *const component;

        bool dirty;
        char *state; // The selected option
        uint8_t maxOptions;
        uint8_t optionsAdded;
        char **options;

    public:

        /// Constructor without device
        HASelect(const char *unique_id, const char *name, uint8_t max_options);
        HASelect(const char *unique_id, const char *name, uint8_t max_options,
            const char *options[]);

        /// Constructor with device
        HASelect(const char *unique_id, const char *name, HADevice& device,
            uint8_t max_options);
        HASelect(const char *unique_id, const char *name, HADevice& device,
            uint8_t max_options, const char *options[]);


        void addOption(const char *option);

        inline bool isDirty() {return this->dirty;};
        const char * getState() {return this->state;};

        void setState(const char *state);

        void onConnect(PubSubClient * client);
        void onReceivedTopic(PubSubClient * client, byte *payload, unsigned int length);
        void sendState(PubSubClient * client);

};

#endif