#include <iostream>
#include "BeMqttCallBack.h"
#include "BeMqttClient.h"

BeMqttCallBack::BeMqttCallBack(BeMqttClient *client)
{
    this->classname = __func__;
    this->_mqttclient = client;
    this->delegate_CBConnected = NULL;
    this->delegate_CBConnectionLost = NULL;
    this->delegate_CBMessageArrived = NULL;
    this->delegate_CBDeliveryComplete = NULL;
    this->delegate_ALOnFailure = NULL;
    this->delegate_ALOnSuccess = NULL;
    cout << "<" << (this) << "> " << this->classname << " created." << endl; 
}

BeMqttCallBack::~BeMqttCallBack()
{
    this->delegate_CBConnected = NULL;
    this->delegate_CBConnectionLost = NULL;
    this->delegate_CBMessageArrived = NULL;
    this->delegate_CBDeliveryComplete = NULL;
    this->delegate_ALOnFailure = NULL;
    this->delegate_ALOnSuccess = NULL;
    cout << "<" << (this) << "> ~" << this->classname << endl;
}

void BeMqttCallBack::connected(const std::string &cause)
{
    if (static_cast<int>(this->verbose) >= 2)
    {
        if (this->_mqttclient != NULL)
        {
            std::cout << "[" << this->_mqttclient->getClientID() << "] " << this->_mqttclient->classname
                      << " Connected !" << std::endl;
        }
    }

    //reconnect
    if (this->_mqttclient->getSubscribedTopic() != "")
    {
        this->_mqttclient->subscribe(this->_mqttclient->getSubscribedTopic(), QosLevel::QoSLv1);
    }
    if (this->delegate_CBConnected != NULL)
    {
        this->delegate_CBConnected(this->_mqttclient, cause);
    }
}

void BeMqttCallBack::connection_lost(const std::string &cause)
{
    if (static_cast<int>(this->verbose) >= 1)
    {
        if (this->_mqttclient != NULL)
        {
            std::cout << "ERROR: [" << this->_mqttclient->getClientID() << "] " << this->_mqttclient->classname
                      << " Connection Lost !" << std::endl;
        }
    }
    if (this->delegate_CBConnectionLost != NULL)
    {
        this->delegate_CBConnectionLost(this->_mqttclient, cause);
    }
}

void BeMqttCallBack::message_arrived(mqtt::const_message_ptr msg)
{
    if (static_cast<int>(this->verbose) >= 2)
    {
        if (this->_mqttclient != NULL)
        {
            cout << "[" << this->_mqttclient->getClientID() << "] " << this->_mqttclient->classname << " :" << endl;
            std::cout << "\tMessage arrived:" << endl;
        }
        std::cout << "\ttopic: '" << msg->get_topic() << "' "
                  << "  message: '" << msg->to_string() << "'" << std::endl;
    }
    if (this->delegate_CBMessageArrived != NULL)
    {
        this->delegate_CBMessageArrived(this->_mqttclient, msg);
    }
}

void BeMqttCallBack::delivery_complete(mqtt::delivery_token_ptr token)
{
    if (static_cast<int>(this->verbose) >= 2)
    {
        if (this->_mqttclient != NULL)
        {
            cout << "[" << this->_mqttclient->getClientID() << "] " << this->_mqttclient->classname << " :" << endl;
        }
        std::cout << "\tDeliveryComplete:" << (token ? token->get_message()->get_payload_str() : "NULL") << std::endl;
    }
    if (this->delegate_CBDeliveryComplete != NULL)
    {
        this->delegate_CBDeliveryComplete(this->_mqttclient, token);
    }
}

void BeMqttCallBack::on_failure(const mqtt::token &tok)
{
    if (static_cast<int>(this->verbose) >= 1)
    {
        if (this->_mqttclient != NULL)
        {
            std::cout << "ERROR: [" << this->_mqttclient->getClientID() << "] " << this->_mqttclient->classname
                      << " On Failure (connection attempt failed) !" << std::endl;
        }
    }
    if (this->delegate_ALOnFailure != NULL)
    {
        this->delegate_ALOnFailure(this->_mqttclient, tok);
    }
}

void BeMqttCallBack::on_success(const mqtt::token &tok)
{
    if (static_cast<int>(this->verbose) >= 2)
    {
        if (this->_mqttclient != NULL)
        {
            cout << "<" << (this->_mqttclient) << "> " << this->_mqttclient->classname << " [" << this->_mqttclient->getClientID() << "] On Success:" << endl;
        }
        if (tok.get_message_id() <= 0)
        {
            std::cout << "\tOn Success for FIRST token: [" << tok.get_message_id() << "]" << std::endl;
        }
        else
        {
            std::cout << "\tOn Success token: [" << tok.get_message_id() << "]" << std::endl;
        }

        auto top = tok.get_topics();
        if (top && !top->empty())
        {
            std::cout << "\tSuccess Token topic id: [" << tok.get_message_id() << "] topic: "
                      << (*top)[0] << std::endl;
        }
    }
    if (this->delegate_ALOnSuccess != NULL)
    {
        this->delegate_ALOnSuccess(this->_mqttclient, tok);
    }
}