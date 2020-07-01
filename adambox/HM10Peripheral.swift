//
//  ParticlePeripheral.swift
//  adambox
//
//  Created by Luis Garcia on 14.06.20.
//  Copyright © 2020 Luis Garcia. All rights reserved.
//

import CoreBluetooth
import Foundation

class HM10Peripheral: NSObject {
    
    public static let hm10ServiceUUID = CBUUID.init(string: "FFE0")
    public static let hm10CharacteristicUUID = CBUUID.init(string: "FFE1")
}
