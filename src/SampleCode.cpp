#include "../include/BeMqttClient.h"
#include <iostream>
#include <unistd.h>

uint64_t timestamp()
{
    auto now = std::chrono::system_clock::now();
    auto tse = now.time_since_epoch();
    auto msTm = std::chrono::duration_cast<std::chrono::milliseconds>(tse);
    return uint64_t(msTm.count());
}

//Callback : connected
void v_Connected(BeMqttClient *client, const std::string &cause)
{
    //Optional: implement callback code
    return;
    if (client != NULL)
    {
        cout << "<" << (client) << "> " << client->classname << " [" << client->getClientID() << "] :" << endl;
    }
    std::cout << "\tConnected: [" << cause << "]" << std::endl;
}

//Callback : connection lost
void v_ConnectionLost(BeMqttClient *client, const std::string &cause)
{
    //Optional: implement callback code
    return;
    if (client != NULL)
    {
        cout << "<" << (client) << "> " << client->classname << " [" << client->getClientID() << "] ConnectionLost:" << endl;
    }
    std::cout << "\tConnectionLost: [" << cause << "]" << std::endl;
    if (!cause.empty())
    {
        std::cout << "\tcause: " << cause << std::endl;
    }
}

//Callback : message arrival
void v_MsgArrived(BeMqttClient *client, mqtt::const_message_ptr msg)
{
    //Optional: implement callback code
    if (client != NULL)
    {
        cout << "[" << client->getClientID() << "] " << client->classname << " :" << endl;
        std::cout << "\tMessage arrived at " << timestamp() << ":" <<endl;
    }
    std::cout << "\ttopic: '" << msg->get_topic() << "' "
              << "  message: '" << msg->to_string() << "'" << std::endl;
}

void v_DeliveryComplete(BeMqttClient *client, mqtt::delivery_token_ptr token)
{
    //Optional: implement callback code
    return;
    if (client != NULL)
    {
        cout << "[" << client->getClientID() << "] " << client->classname << " :" << endl;
    }
    std::cout << "\tDeliveryComplete:" << (token ? token->get_message()->get_payload_str() : "NULL") << std::endl;
}

void v_onSuccess(BeMqttClient *client, const mqtt::token &tok)
{
    //Optional: implement callback code
    if (client != NULL)
    {
        cout << "<" << (client) << "> " << client->classname << " [" << client->getClientID() << "] On Success:" << endl;
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
                  << (*top)[0] << "', ..." << std::endl;
    }
}
void v_OnFailure(BeMqttClient *client, const mqtt::token &tok)
{
    //Optional: implement callback code
    return;
    if (client != NULL)
    {
        cout << "<" << (client) << "> " << client->classname << " [" << client->getClientID() << "] On Failure:" << endl;
    }
    cout << "\n\tListener failure for token: [" << tok.get_message_id() << "]"
         << tok.get_type() << endl;
}

void printCommand(bool listtitle)
{
    if (listtitle)
    {
        std::cout << "Command List :" << std::endl;
    }
    std::cout << " - Press [P] [Enter] to Publish" << std::endl;
    std::cout << " - Press [T] [Enter] to Print Datetime." << std::endl;
    std::cout << " - Press [Q] [Enter] to Quit." << std::endl;
    std::cout << "Input command : " << std::flush;
}

int main()
{
    std::string clientid = "ClientID-001";
    BeMqttClient client(clientid,BeMqttOption::BrokerAddr,BeMqttOption::BrokerUsername,BeMqttOption::BrokerPassword);
    client.setVerbose(Verbose::VerboseError);

    //client.setConnected(v_Connected);
    //client.setConnectionLost(v_ConnectionLost);
    
    client.setMessageArrived(v_MsgArrived);
    client.setDeliveryComplete(v_DeliveryComplete);

    //client.setOnSuccess(v_onSuccess);
    //client.setOnFailure(v_OnFailure);

    client.connect();
    client.subscribe("#", QosLevel::QoSLv1);
    client.publish("T1","Test");

    //BeMqttClient cli2("AB");
    //cli2.setMessageArrived(v_MsgArr2);
    //cli2.connect();
    //cli2.subscribe("T2");

    printCommand(true);
    //while (std::tolower(std::cin.get()) != 'q')
    //;
    //鍵盤輸入 Q 離開
    std::string inputString;
    bool doLoop = true;
    while (doLoop)
    {

        //char q = std::tolower(std::cin.get());
        getline(std::cin, inputString);
        for (auto &c : inputString)
        {
            c = tolower(c);
        }
        if (inputString == "q" || inputString == "exit" || inputString == "quit")
        {
            doLoop = false;
        }
        else if (inputString == "p")
        {
            int i = 1;
            for (i = 1; i <= 100; i++)
            {
                char msg[150];
                sprintf(msg, "No. %i : timestamp %ld", i, timestamp());
                client.publish("T1", msg, QosLevel::QoSLv1);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            printCommand(true);
        }
        else if (inputString == "t"){
            int i = 1;
            for (i = 1; i <= 40; i++)
            {
                std::cout << "No." << i << " : Timestamp " << timestamp() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            printCommand(true);
        }
        else
        {
            std::cout << "Error!! Invalid input : " << inputString << std::endl;
            printCommand(false);
        }
    };
    usleep(1);
    client.disConnect();
}