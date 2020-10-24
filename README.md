## MQTT client for c++ Sample (BeMqttClient)<br>
##### A customize MQTT Client for C++ based on Paho.mqtt.cpp. Tested on macOS & Linux #####

**Feature:**
- Class **`BeMqttClient.h`** implemented from `mqtt/async_client`. More easy to use.

**Dependency libraries:**
- [paho.mqtt.c](https://github.com/eclipse/paho.mqtt.c) : Eclipse Paho C Client Library for the MQTT Protocol
- [paho.mqtt.cpp](https://github.com/eclipse/paho.mqtt.c) : Eclipse Paho MQTT C++ Client Library<br>

Install script:
```shell
sudo apt install git build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev doxygen graphviz libcppunit-dev -y
cd ~
git clone https://github.com/eclipse/paho.mqtt.cpp
cd paho.mqtt.cpp
./install_paho_mqtt_c.sh
cmake -Bbuild -H. -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=FALSE
sudo cmake --build build/ --target install
sudo ldconfig
```

**How to use:**
1. Put your code file(s) `.cpp`  in **`src/`** directory.
2. Just add `#include <BeMqttClient.h>` or  `#include "../include/BeMqttClient.h"`
3. **`make run`** in console to complie & launch your code.
4. **(Optinal*)** Recommand to open this project via [Microsoft Visual Studio Code](https://code.visualstudio.com/download) because this project was configured for this IDE tool.

**Sample Code:**
```cpp
#include <iostream>
#include "../include/BeMqttClient.h"

void v_MsgArrived(BeMqttClient *client, mqtt::const_message_ptr msg)
{
   //implement 
   std::cout << "Message arrived:" << endl;
   std::cout << "\ttopic: '" << msg->get_topic() << "' "
             << "\tmessage: '" << msg->to_string() << "'" << std::endl;
}
int main()
{
   std::string BrokerAddr = "tcp://localhost:1883"; //Your wrking MQTT broker IP or DNS with port number.
   std::string clientid = "ClientTest";

   BeMqttClient client1(clientid,BrokerAddr);
   client1.setMessageArrived(v_MsgArrived); //Message arrived callback.
   client1.connect();
   client1.subscribe("#", QosLevel::QoSLv1);
   client1.publish("TestTpoic","Test");

   std::cout << "Press 'q' [Enter] to quit" << std::endl;
   while (std::tolower(std::cin.get()) != 'q');
   client1.disConnect();
}
```
Console output:
```
<0x563ee1e02330> BeMqttCallBack created.
<0x7ffc962b13b0> [ClientTest] BeMqttClient created.
[ClientTest] BeMqttClient Connecting to [tcp://localhost:1883]....
[ClientTest] BeMqttClient Subscribe [#].....OK!
Press 'q' [Enter] to quit
Message arrived:
        topic: 'TestTpoic'      message: 'Test'
q
[ClientTest] BeMqttClient disConnect()...
<0x563ee1e02330> ~BeMqttCallBack
<0x7ffc962b13b0> [ClientTest] ~BeMqttClient
```
For more detail, see [src/SampleCode.cpp](src/SampleCode.cpp)

