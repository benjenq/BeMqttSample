#ifndef __BEMQTTCALLBACK_H__
#define __BEMQTTCALLBACK_H__

#include <iostream>
#include <mqtt/async_client.h>

using namespace std;

enum class Verbose : int {
    VerboseNone = 0,
    VerboseError = 1,
    VerboseAll = 2
};

class BeMqttClient;
class BeMqttCallBack : public virtual mqtt::callback , public virtual mqtt::iaction_listener
{
private:
    BeMqttClient *_mqttclient;
    /** mqtt::callback */

    void connected(const std::string& cause) override;
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr msg) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

    /** mqtt::iaction_listener*/

    void on_failure(const mqtt::token& tok) override  ;
    void on_success(const mqtt::token& tok) override  ;

public:
    std::string classname;
    Verbose verbose = Verbose::VerboseNone;
    BeMqttCallBack(BeMqttClient *client);
    ~BeMqttCallBack();
    void (*delegate_CBConnected)(BeMqttClient *client, const std::string& cause);
    void (*delegate_CBConnectionLost)(BeMqttClient *client, const std::string& cause);
    void (*delegate_CBMessageArrived)(BeMqttClient *client, mqtt::const_message_ptr msg);
    void (*delegate_CBDeliveryComplete)(BeMqttClient *client, mqtt::delivery_token_ptr token);

    void (*delegate_ALOnFailure)(BeMqttClient *client, const mqtt::token& tok);
    void (*delegate_ALOnSuccess)(BeMqttClient *client, const mqtt::token& tok);
};



#endif // __EMMQTTCALLBACK_H__