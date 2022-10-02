#include "BLE.h"
#include <NimBLEDevice.h>

// String DeviceName = "Si-SCA 1";//+String((uint32_t)ESP.getEfuseMac(), HEX);

// static NimBLEServer* pServer;
// bool deviceConnected = false;
// bool oldDeviceConnected = false;
// uint32_t value = 0;

// // See the following for generating UUIDs:
// // https://www.uuidgenerator.net/
// static BLEUUID service_HR_UUID(BLEUUID((uint16_t)0x180D));
// static BLEUUID char_HR_UUID(BLEUUID((uint16_t)0x2A37));
// static BLEUUID descriptor_HR_UUID(BLEUUID((uint16_t)0x2901));

// static BLEUUID service_BATT_UUID(BLEUUID((uint16_t)0x180F));
// static BLEUUID char_BATT_UUID(BLEUUID((uint16_t)0x2A19));
// static BLEUUID descriptor_BATT_UUID(BLEUUID((uint16_t)0x2901));

// static BLEUUID service_ACL_UUID(BLEUUID((uint16_t)0x181A));
// static BLEUUID char_ACL_UUID(BLEUUID((uint16_t)0x290C));
// static BLEUUID descriptor_ACL_UUID(BLEUUID((uint16_t)0x2713));

// class ServerCallbacks: public NimBLEServerCallbacks {
//     void onConnect(NimBLEServer* pServer) {
//         Serial.println("Client connected");
//         Serial.println("Multi-connect support: start advertising");
//         NimBLEDevice::startAdvertising();
//     };
//     /** Alternative onConnect() method to extract details of the connection.
//      *  See: src/ble_gap.h for the details of the ble_gap_conn_desc struct.
//      */
//     void onConnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) {
//         Serial.print("Client address: ");
//         Serial.println(NimBLEAddress(desc->peer_ota_addr).toString().c_str());
//         /** We can use the connection handle here to ask for different connection parameters.
//          *  Args: connection handle, min connection interval, max connection interval
//          *  latency, supervision timeout.
//          *  Units; Min/Max Intervals: 1.25 millisecond increments.
//          *  Latency: number of intervals allowed to skip.
//          *  Timeout: 10 millisecond increments, try for 5x interval time for best results.
//          */
//         pServer->updateConnParams(desc->conn_handle, 24, 48, 0, 60);
//     };
//     void onDisconnect(NimBLEServer* pServer) {
//         Serial.println("Client disconnected - start advertising");
//         NimBLEDevice::startAdvertising();
//     };
//     void onMTUChange(uint16_t MTU, ble_gap_conn_desc* desc) {
//         Serial.printf("MTU updated: %u for connection ID: %u\n", MTU, desc->conn_handle);
//     };

// /********************* Security handled here **********************
// ****** Note: these are the same return values as defaults ********/
//     uint32_t onPassKeyRequest(){
//         Serial.println("Server Passkey Request");
//         /** This should return a random 6 digit number for security
//          *  or make your own static passkey as done here.
//          */
//         return 123456;
//     };

//     bool onConfirmPIN(uint32_t pass_key){
//         Serial.print("The passkey YES/NO number: ");Serial.println(pass_key);
//         /** Return false if passkeys don't match. */
//         return true;
//     };

//     void onAuthenticationComplete(ble_gap_conn_desc* desc){
//         /** Check that encryption was successful, if not we disconnect the client */
//         if(!desc->sec_state.encrypted) {
//             NimBLEDevice::getServer()->disconnect(desc->conn_handle);
//             Serial.println("Encrypt connection failed - disconnecting client");
//             return;
//         }
//         Serial.println("Starting BLE work!");
//     };
// };

// /** Handler class for characteristic actions */
// class CharacteristicCallbacks: public NimBLECharacteristicCallbacks {
//     void onRead(NimBLECharacteristic* pCharacteristic){
//         Serial.print(pCharacteristic->getUUID().toString().c_str());
//         Serial.print(": onRead(), value: ");
//         Serial.println(pCharacteristic->getValue().c_str());
//     };

//     void onWrite(NimBLECharacteristic* pCharacteristic) {
//         Serial.print(pCharacteristic->getUUID().toString().c_str());
//         Serial.print(": onWrite(), value: ");
//         Serial.println(pCharacteristic->getValue().c_str());
//     };
//     /** Called before notification or indication is sent,
//      *  the value can be changed here before sending if desired.
//      */
//     void onNotify(NimBLECharacteristic* pCharacteristic) {
//         Serial.println("Sending notification to clients");
//     };


//     /** The status returned in status is defined in NimBLECharacteristic.h.
//      *  The value returned in code is the NimBLE host return code.
//      */
//     void onStatus(NimBLECharacteristic* pCharacteristic, Status status, int code) {
//         String str = ("Notification/Indication status code: ");
//         str += status;
//         str += ", return code: ";
//         str += code;
//         str += ", ";
//         str += NimBLEUtils::returnCodeToString(code);
//         Serial.println(str);
//     };

//     void onSubscribe(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc, uint16_t subValue) {
//         String str = "Client ID: ";
//         str += desc->conn_handle;
//         str += " Address: ";
//         str += std::string(NimBLEAddress(desc->peer_ota_addr)).c_str();
//         if(subValue == 0) {
//             str += " Unsubscribed to ";
//         }else if(subValue == 1) {
//             str += " Subscribed to notfications for ";
//         } else if(subValue == 2) {
//             str += " Subscribed to indications for ";
//         } else if(subValue == 3) {
//             str += " Subscribed to notifications and indications for ";
//         }
//         str += std::string(pCharacteristic->getUUID()).c_str();

//         Serial.println(str);
//     };
// };

// /** Handler class for descriptor actions */
// class DescriptorCallbacks : public NimBLEDescriptorCallbacks {
//     void onWrite(NimBLEDescriptor* pDescriptor) {
//         std::string dscVal = pDescriptor->getValue();
//         Serial.print("Descriptor witten value:");
//         Serial.println(dscVal.c_str());
//     };

//     void onRead(NimBLEDescriptor* pDescriptor) {
//         Serial.print(pDescriptor->getUUID().toString().c_str());
//         Serial.println(" Descriptor read");
//     };
// };

BLE::BLE(void)
{
}

void BLE::begin(String deviceName)
{
  NimBLEDevice::init(deviceName.c_str());
  _deviceName = deviceName;
  
  // // Create the BLE Server
  // pServer = NimBLEDevice::createServer();
  // pServer->setCallbacks(new ServerCallbacks());

  // // Create the BLE Service
  // NimBLEService* HeartRateService = pServer->createService(service_HR_UUID);

  // // Create a BLE Characteristic
  // NimBLECharacteristic* HeartRateCharacteristic = HeartRateService->createCharacteristic(
  //                     char_HR_UUID,
  //                     NIMBLE_PROPERTY::READ |
  //                     NIMBLE_PROPERTY::WRITE|
  //                     NIMBLE_PROPERTY::NOTIFY
  //                   );
  // NimBLEDescriptor* HeartRateDescriptor = HeartRateCharacteristic->createDescriptor(
  //                                 descriptor_HR_UUID,
  //                                 NIMBLE_PROPERTY::READ |
  //                                 NIMBLE_PROPERTY::NOTIFY, // only allow writing if paired / encrypted
  //                                 20
  //                               );
  // HeartRateDescriptor->setValue("HR Measurement");
  // HeartRateDescriptor->setCallbacks(&dscCallbacks);

  // NimBLEService* BattMonService = pServer->createService(service_BATT_UUID);

  // NimBLECharacteristic* BattMonCharacteristic = BattMonService->createCharacteristic(
  //                     char_BATT_UUID,
  //                     NIMBLE_PROPERTY::READ |
  //                     NIMBLE_PROPERTY::NOTIFY
  //                   );
  // NimBLEDescriptor* BattMonDescriptor = BattMonCharacteristic->createDescriptor(
  //                                 descriptor_BATT_UUID,
  //                                 NIMBLE_PROPERTY::READ |
  //                                 NIMBLE_PROPERTY::NOTIFY, // only allow writing if paired / encrypted
  //                                 20
  //                               );
  // BattMonDescriptor->setValue("Battery Level");
  // BattMonDescriptor->setCallbacks(&dscCallbacks);

  // NimBLEService* AccelService = pServer->createService(service_ACL_UUID);

  // NimBLECharacteristic* AccelCharacteristic = AccelService->createCharacteristic(
  //                     char_ACL_UUID,
  //                     NIMBLE_PROPERTY::READ |
  //                     NIMBLE_PROPERTY::NOTIFY
  //                   );
  // NimBLEDescriptor* AccelDescriptor = AccelCharacteristic->createDescriptor(
  //                                 descriptor_ACL_UUID,
  //                                 NIMBLE_PROPERTY::READ |
  //                                 NIMBLE_PROPERTY::NOTIFY, // only allow writing if paired / encrypted
  //                                 20
  //                               );
  // AccelDescriptor->setValue("MPU6050 Status");
  // AccelDescriptor->setCallbacks(&dscCallbacks);

  // // Start the service
  // HeartRateService->start();
  // BattMonService->start();
  // AccelService->start();

  // // Start advertising
  // NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  // pAdvertising->addServiceUUID(HeartRateService->getUUID());
  // pAdvertising->addServiceUUID(BattMonService->getUUID());
  // pAdvertising->addServiceUUID(AccelService->getUUID());
  // pAdvertising->setScanResponse(false);
  // pAdvertising->start();
}

void BLE::sendData(void)
{
  // if(pServer->getConnectedCount()) {
  //     int max33[3];
  //     max33[0] |= value;
  //     max33[1] |= 1;
  //     max33[2] |= 10;

  //     NimBLEService* pSvc1 = pServer->getServiceByUUID(service_BATT_UUID);
  //     NimBLECharacteristic* BattMonCharacteristic = pSvc1->getCharacteristic(char_BATT_UUID);
  //     BattMonCharacteristic->setValue((uint8_t*)&value, 2);
  //     BattMonCharacteristic->notify();
  //     NimBLEService* pSvc2 = pServer->getServiceByUUID(service_HR_UUID);
  //     NimBLECharacteristic* HeartRateCharacteristic = pSvc2->getCharacteristic(char_HR_UUID);
  //     HeartRateCharacteristic->setValue((uint8_t*)&max33, 3);
  //     HeartRateCharacteristic->notify();
  //     NimBLEService* pSvc3 = pServer->getServiceByUUID(service_ACL_UUID); 
  //     NimBLECharacteristic* AccelCharacteristic = pSvc3->getCharacteristic(char_ACL_UUID);
  //     AccelCharacteristic->setValue((uint8_t*)&value, 2);
  //     AccelCharacteristic->notify();
  //     value++;
  //     digitalWrite(2, value%2);
  //     Serial.println(value);
  //     if(value>100)value=0;
  //     delay(50); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
  // }
}