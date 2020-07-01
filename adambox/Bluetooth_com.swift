//
//  bluetooth_com.swift
//  adambox
//
//  Created by Luis Garcia on 14.06.20.
//  Copyright © 2020 Luis Garcia. All rights reserved.
//

import Foundation
import UIKit
import CoreBluetooth
import SwiftUI



class BLEService: NSObject, CBPeripheralDelegate, CBCentralManagerDelegate, ObservableObject {
    var centralManager: CBCentralManager!
    private var peripheral: CBPeripheral!
     var hm10Characteristic: CBCharacteristic!
    //is device connected? - global variable
    @Published var connected: Bool = false

    
    var scanned_BLE_devices: [String] = []

    func startCentralManager() {
        self.centralManager = CBCentralManager(delegate: self, queue: nil)
        print("Central Manager State: \(self.centralManager.state)")
        DispatchQueue.main.asyncAfter(deadline: .now() + 1) {
            self.centralManagerDidUpdateState(self.centralManager)
        }
    }
    
    func sendData(data: Data?) {
        if self.hm10Characteristic != nil && self.peripheral != nil{
            //self.peripheral.writeValue(data!, for: self.hm10Characteristic, type: .withoutResponse)
            self.peripheral.writeValue(data!, for: self.hm10Characteristic, type: .withoutResponse)
        }
    }
    
    func connectToDevice() {
        centralManager.scanForPeripherals(withServices: [HM10Peripheral.hm10ServiceUUID],
        options: [CBCentralManagerScanOptionAllowDuplicatesKey : true])
    }
    
    func disconnectFromDevice() {
        if self.peripheral != nil {
            self.centralManager.cancelPeripheralConnection(self.peripheral)
        }
    }
    
    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        if peripheral == self.peripheral {
            print("Disconnected to your Soundbox")
            self.connected = false
        }
    }
    
    
    // If we're powered on, start scanning
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
                        
        switch central.state {
        case .unknown:
            print("unknown")
        case .resetting:
            print("resetting")
        case .unsupported:
            print("unsupported")
        case .unauthorized:
            print("unauthorized")
        case .poweredOff:
            self.connected = false
            print("powered off")
        case .poweredOn:
            print("powered on")
            print("Central scanning for", HM10Peripheral.hm10ServiceUUID);
        default:
            self.connected = false
            print("unknown error")
        }
    }
    
    // Handles the result of the scan
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {

        // We've found it so stop scan
        self.centralManager.stopScan()

        // Copy the peripheral instance
        self.peripheral = peripheral
        self.peripheral.delegate = self

        // Connect!
        self.centralManager.connect(self.peripheral, options: nil)

    }
    
    // The handler if we do connect succesfully
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        if peripheral == self.peripheral {
            print("Connected to your Soundbox")
            peripheral.discoverServices([HM10Peripheral.hm10ServiceUUID])
            self.connected = true
        }
    }
    
    // Handles discovery event
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        if let services = peripheral.services {
            for service in services {
                if service.uuid == HM10Peripheral.hm10ServiceUUID {
                    print("Data service found")
                    //Now kick off discovery of characteristics
                    peripheral.discoverCharacteristics([HM10Peripheral.hm10CharacteristicUUID], for: service)
                    return
                }
            }
        }
    }
    
    // Handling discovery of characteristics
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        if let characteristics = service.characteristics {
            for characteristic in characteristics {
                if characteristic.uuid == HM10Peripheral.hm10CharacteristicUUID {
                    self.hm10Characteristic = characteristic
                    print("Data characteristic found")
                }
            }
        }
    }
    
    
}
