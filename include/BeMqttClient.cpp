#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include "BeMqttClient.h"

const string BeMqttOption::BrokerAddr = "tcp://localhost:1883";
const string BeMqttOption::BrokerUsername = "username";
const string BeMqttOption::BrokerPassword = "password";

BeMqttClient::BeMqttClient(std::string clientid)
{
    this->classname = __func__;
    this->_clientid = clientid;
    this->_brokerAddr = BeMqttOption::BrokerAddr;
    this->_connOpts.set_user_name(BeMqttOption::BrokerUsername);
    this->_connOpts.set_password(BeMqttOption::BrokerPassword);
    this->_connOpts.set_keep_alive_interval(20);
    this->_connOpts.set_clean_session(true);
    this->_connOpts.set_automatic_reconnect(true);
    this->_mqttaClient = new mqtt::async_client(this->_brokerAddr, this->_clientid);
    this->_isConnected = false;
    this->_mqttCB = new BeMqttCallBack(this);
    this->_mqttaClient->set_callback(*_mqttCB);

    cout << "<" << (this) << "> [" << this->_clientid << "] " << this->classname << " Created." << endl;
    cout << "<" << (this->_mqttCB) << "> " << this->_mqttCB->classname << " created." << endl;  

}

BeMqttClient::BeMqttClient(std::string clientid, std::string brokerAddr) 
{
    this->classname = __func__;
    this->_clientid = clientid;
    this->_brokerAddr = brokerAddr;
    this->_connOpts.set_user_name(BeMqttOption::BrokerUsername);
    this->_connOpts.set_password(BeMqttOption::BrokerPassword);
    this->_connOpts.set_keep_alive_interval(20);
    this->_connOpts.set_clean_session(true);
    this->_connOpts.set_automatic_reconnect(true);
    this->_mqttaClient = new mqtt::async_client(this->_brokerAddr , this->_clientid);
    this->_isConnected = false;
    this->_mqttCB = new BeMqttCallBack(this);
    this->_mqttaClient->set_callback(*_mqttCB);

    cout << "<" << (this) << "> [" << this->_clientid << "] " << this->classname << " created." << endl;
}

BeMqttClient::BeMqttClient(std::string clientid, std::string brokerAddr, std::string brokerUsername, std::string brokerPassword) 
{
    this->classname = __func__;
    this->_clientid = clientid;
    this->_brokerAddr = brokerAddr;
    this->_connOpts.set_user_name(brokerUsername);
    this->_connOpts.set_password(brokerPassword);
    this->_connOpts.set_keep_alive_interval(20);
    this->_connOpts.set_clean_session(true);
    this->_connOpts.set_automatic_reconnect(true);
    this->_mqttaClient = new mqtt::async_client(this->_brokerAddr, this->_clientid);
    this->_isConnected = false;
    this->_mqttCB = new BeMqttCallBack(this);
    this->_mqttaClient->set_callback(*_mqttCB);    

    cout << "<" << (this) << "> [" << this->_clientid << "] " << this->classname << " Created." << endl;
    
}

BeMqttClient::~BeMqttClient()
{
    this->_isConnected = false;
    delete _mqttCB;
    cout << "<" << (this) << "> "
         << "[" << this->_clientid << "] "
         << "~" << this->classname << endl;
}

std::string BeMqttClient::getClientID()
{
    return this->_clientid;
}
std::string BeMqttClient::getSubscribedTopic()
{
    return this->_subscribedTopic;
}

void BeMqttClient::setVerbose(Verbose verbose) 
{
    this->_mqttCB->verbose = verbose;
}

bool BeMqttClient::connect()
{
    bool success = true;
    try
    {
        std::cout << "[" << this->getClientID() << "] " << this->classname
                  << " Connecting to [" << this->_brokerAddr << "]...." << std::endl;
        this->_mqttaClient->connect(_connOpts, nullptr, *_mqttCB)->wait_for(std::chrono::milliseconds(1500));
        //_mqttaClient->connect(_connOpts)->wait_for(std::chrono::seconds(3));
    }
    catch (const mqtt::exception &e)
    {
        std::cerr << "ERROR: Unable to connect to MQTT server: '" << e.to_string() << std::endl;
        success = false;
    }
    this->_isConnected = success;
    return success;
}

bool BeMqttClient::isConnected()
{
    return this->_isConnected;
}

void BeMqttClient::disConnect()
{
    try
    {
        std::cout << "[" << this->getClientID() << "] " << this->classname << " disConnect()..." << endl;
        this->_mqttaClient->disconnect(nullptr, *_mqttCB)->wait_for(std::chrono::milliseconds(2500));
        this->_isConnected = false;
    }
    catch (const mqtt::exception &e)
    {
        std::cerr << "ERROR: Unable to disConnect from MQTT server: '" << e.to_string() << std::endl;
    }
}

void BeMqttClient::publish(std::string TOPIC, const char *MESSAGE)
{
    publish(TOPIC, MESSAGE, QosLevel::QoSLv0);
}

void BeMqttClient::publish(std::string TOPIC, const char *MESSAGE, QosLevel QoS)
{
    try
    {
        //std::cout<<"publish ["<<TOPIC<<":"<<MESSAGE<<"] to Broker";
        mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, MESSAGE);
        //BeMqttActionListener *lis = new BeMqttActionListener();
        //_mqttaClient->publish(pubmsg, nullptr, *_mqttAL)->wait_for(std::chrono::seconds(3));
        //_mqttaClient->publish(TOPIC,MESSAGE,strlen(MESSAGE)+1,BeMqttOption::QOS,false);//,nullptr, *this->_mqttAL);
        this->_mqttaClient->publish(TOPIC, MESSAGE, strlen(MESSAGE) + 1, static_cast<int>(QoS), false, nullptr, *_mqttCB)->wait_for(std::chrono::milliseconds(1500));
        //std::cout<<"...OK!"<<endl;
    }
    catch (const mqtt::exception &e)
    {
        std::cout << "ERROR: Unable to publish " << e.to_string() << std::endl;
    }
}

void BeMqttClient::subscribe(std::string TOPIC)
{
    subscribe(TOPIC, QosLevel::QoSLv0);
}

void BeMqttClient::subscribe(std::string TOPIC, QosLevel QoS)
{
    try
    {
        std::cout << "[" << this->getClientID() << "] " << this->classname
                  << " Subscribe [" << TOPIC << "]..." << std::flush;
        this->_mqttaClient->subscribe(TOPIC, static_cast<int>(QoS), nullptr, *_mqttCB);
        cout << "..OK!" << endl;
        this->_subscribedTopic = TOPIC;
    }
    catch (const mqtt::exception &e)
    {
        std::cout << "ERROR: Unable to subscribe : '" << TOPIC << e.to_string() << std::endl;
    }
}

void BeMqttClient::unsubscribe(std::string TOPIC)
{
    try
    {
        cout << "Unsubscribe [" << TOPIC << "]...";
        this->_mqttaClient->unsubscribe(TOPIC, nullptr, *_mqttCB)->wait_for(std::chrono::milliseconds(1500));
        cout << "..OK!" << endl;
        this->_subscribedTopic = "";
    }
    catch (const mqtt::exception &e)
    {
        std::cout << "\nERROR: Unable to Unsubscribe : '" << TOPIC << e.to_string() << std::endl;
    }
}

void BeMqttClient::setConnected(void (*voidConnected)(BeMqttClient *client, const std::string &cause))
{
    this->_mqttCB->delegate_CBConnected = voidConnected;
}

void BeMqttClient::setConnectionLost(void (*voidConnectionLost)(BeMqttClient *client, const std::string &cause))
{
    this->_mqttCB->delegate_CBConnectionLost = voidConnectionLost;
}

void BeMqttClient::setMessageArrived(void (*voidMsgArrived)(BeMqttClient *client, mqtt::const_message_ptr msg))
{
    this->_mqttCB->delegate_CBMessageArrived = voidMsgArrived;
}

void BeMqttClient::setDeliveryComplete(void (*voidDeliveryComplete)(BeMqttClient *client, mqtt::delivery_token_ptr token))
{
    this->_mqttCB->delegate_CBDeliveryComplete = voidDeliveryComplete;
}

void BeMqttClient::setOnFailure(void (*voidOnFailure)(BeMqttClient *client, const mqtt::token &tok))
{
    this->_mqttCB->delegate_ALOnFailure = voidOnFailure;
}

void BeMqttClient::setOnSuccess(void (*voidOnSuccess)(BeMqttClient *client, const mqtt::token &tok))
{
    this->_mqttCB->delegate_ALOnSuccess = voidOnSuccess;
}