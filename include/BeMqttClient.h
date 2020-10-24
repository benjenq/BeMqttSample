#ifndef __BEMQTTCLIENT_H__
#define __BEMQTTCLIENT_H__

#include <iostream>
#include <mqtt/async_client.h>
#include "BeMqttCallBack.h"

enum class QosLevel : int {
    QoSLv0 = 0,
    QoSLv1 = 1,
    QoSLv2 = 2
};

struct BeMqttOption
{
private:
    /* data */
public:
    BeMqttOption(/* args */);
    ~BeMqttOption();
    static const string BrokerAddr;
    static const string BrokerUsername;
    static const string BrokerPassword;
};

class BeMqttCallBack;

/**
 * @brief BeMqttClient: MQTT client object using mqtt::async_client. Easy use and support autoreconnect.
 * 
 */
class BeMqttClient
{
private:
    bool _isConnected; 
    std::string _clientid;
    std::string _brokerAddr;
    std::string _subscribedTopic = "";
    mqtt::async_client *_mqttaClient;
    mqtt::connect_options _connOpts;
    BeMqttCallBack *_mqttCB;
    /* data */
   
public: 
    const char* classname;
    /**
     * @brief Construct a new BeMqttClient object
     * 
     * @param clientId Client identifier string
     */
    BeMqttClient(std::string clientId);
    /**
     * @brief Construct a new BeMqttClient object
     * 
     * @param clientId Client identifier string
     * @param brokerAddr Broker ip or dns, format ex.: "tcp://192.168.1.200:1883"
     */
    BeMqttClient(std::string clientid, std::string brokerAddr);

    /**
     * @brief Construct a new BeMqttClient object
     * 
     * @param clientId Client identifier string
     * @param brokerAddr Broker ip or dns, e.g.: "tcp://192.168.1.200:1883"
     * @param brokerUsername Username for login broker
     * @param brokerPassword Password for login broker
     */
    BeMqttClient(std::string clientId, std::string brokerAddr, std::string brokerUsername, std::string brokerPassword);

    /**
     * @brief Destroy the BeMqttClient object
     * 
     */
    ~BeMqttClient();
    /**
     * @brief Get the Client ID from client 
     * 
     * @return std::string lient ID
     */
    std::string getClientID();

    /**
     * @brief Get the subscribed topic from client 
     * 
     * @return std::string subscribed topic
     */
    std::string getSubscribedTopic();

    /**
     * @brief get client connected or not.
     * 
     * @return true/false  Connected/Not connected
     * 
     */
    bool isConnected();
    
    /**
     * @brief Set client connection verbose mode ON/OFF
     * 
     * @param verbose true:ON, false:OFF
     */
    void setVerbose(Verbose verbose);

    /**
     * @brief Method - Connect to MQTT broker 
     * 
     * @return true | false : Connect success or not
     */
    bool connect();
    /**
     * @brief Method - Disocnnect from MQTT broker 
     * 
     */
    void disConnect();

    /**
     * @brief Publish MESSAGE string to broker on TOPIC with QoS Lv.0
     * 
     * @param TOPIC TOPIC
     * @param MESSAGE MESSAGE
     */
    void publish(std::string TOPIC,const char* MESSAGE);

    /**
     * @brief Method - Publish MESSAGE string to broker on TOPIC with QoS
     * 
     * @param TOPIC TOPIC
     * @param MESSAGE MESSAGE
     * @param QoS QoS Lv: 0,1,2
     */
    void publish(std::string TOPIC,const char* MESSAGE, QosLevel QoS);

    /**
     * @brief Method - Subscribe TOPIC to receive MESSAGE from broker with Qos Lv.0
     * 
     * @param TOPIC TOPIC
     */
    void subscribe(std::string TOPIC);

    /**
     * @brief Method - Subscribe TOPIC to receive MESSAGE from broker with Qos
     * 
     * @param TOPIC TOPIC
     * @param QoS Qos
     */
    void subscribe(std::string TOPIC, QosLevel QoS);

    /**
     * @brief Method - Unsubscribe TOPIC to receive MESSAGE from broker with Qos
     * 
     * @param TOPIC TOPIC
     */
    void unsubscribe(std::string TOPIC);

    /**
     * 
     * @brief (Optional) Set the mqtt::callback callback delegate , triggered after client connected to broker.
     * 
     * @param voidConnected (const std::string& cause)
     */
    void setConnected(void(*voidConnected)(BeMqttClient *client, const std::string& cause));

    /**
     * @brief (Optional) Set the mqtt::callback callback delegate , triggered after client lost connection from broker (broker was down or network issue).
     * 
     * @param voidConnectionLost (const std::string& cause)
     */
    void setConnectionLost(void(*voidConnectionLost)(BeMqttClient *client, const std::string& cause));

    /**
     * @brief (Optional) Set the mqtt::callback callback delegate , triggered when client receive message(topic and payload) from broker.
     * 
     * @param voidMsgArrived (mqtt::const_message_ptr msg)
     */
    void setMessageArrived(void(*voidMsgArrived)(BeMqttClient *client, mqtt::const_message_ptr msg));

    /**
     * @brief (Optional) Set the mqtt::callback callback delegate , triggered when client publish message to broker successful for ensure other client could receive this message. 
     * Qos of published message must be >= 1 or it will not triggered.
     * 
     * @param voidDeliveryComplete (mqtt::delivery_token_ptr token)
    */
    void setDeliveryComplete(void(*voidDeliveryComplete)(BeMqttClient *client, mqtt::delivery_token_ptr token));

    /**
     * @brief (Optional) Set the mqtt::iaction_listener callback delegate, triggered when client got failure after method : connect, publish, subscribe.
     * 
     * @param voidOnFailure (const mqtt::token& tok)
    */
    void setOnFailure(void(*voidOnFailure)(BeMqttClient *client, const mqtt::token& tok));

    /**
     * @brief (Optional) Set the mqtt::iaction_listener callback delegate, triggered when client got success after method : connect, publish, subscribe.
     * 
     * @param voidOnSuccess 
     */
    void setOnSuccess(void(*voidOnSuccess)(BeMqttClient *client, const mqtt::token& tok));

};

#endif // __EMMQTTCLIENT_H__