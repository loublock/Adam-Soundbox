//
//  ContentView.swift
//  adambox
//
//  Created by Luis Garcia on 12.06.20.
//  Copyright © 2020 Luis Garcia. All rights reserved.
//

import SwiftUI
import AVFoundation
import ASCollectionView
import CoreBluetooth
import Combine


//AS Collection View
struct CollectionViewNotConnected: View {
    let ble: BLEService
    @State var dataExample: [String]
    var tools = Tools()
    
    var body: some View
    {
        VStack {
            Button(action: {
                //connect to device
                self.ble.connectToDevice()

            }) {
                Text("Soundbox suchen")
            }
            .frame(minWidth: 0, maxWidth: .infinity)
            .padding()
            .foregroundColor(.black)
            .background(Color.blue)
            .cornerRadius(10)
            .padding(.horizontal, 20)
            
            ASCollectionView(data: dataExample, dataID: \.self) { item, _ in
                Color.red
                    .animation(.default)
                    .overlay(Text("\(self.tools.get_name_from_key(key: item, dict_name: "key_to_label")!)"))
                    .onTapGesture {
                        self.tools.playTrack(key: item)
                }
            .cornerRadius(10)
            }
            .layout {
                .grid(layoutMode: .adaptive(withMinItemSize: 100),
                    itemSize: .absolute(50))
            }
        }

    }
}

//AS Collection View
struct CollectionViewConnected: View {
    let ble: BLEService
    @State var dataExample: [String]
    var tools = Tools()
    
    var body: some View
    {
        VStack {
            Button(action: {
                // kill the connection to the soundbox
                self.ble.disconnectFromDevice()
                
            }) {
                Text("Soundbox verbunden")
            }
            .frame(minWidth: 0, maxWidth: .infinity)
            .padding()
            .foregroundColor(.black)
            .background(Color.green)
            .cornerRadius(10)
            .padding(.horizontal, 20)
            .padding(.vertical, 10)
            
            HStack {
                Button(action: {
                    //soundbox volume down
                    self.ble.sendData(data: "97".data(using: .utf8))

                }) {
                    Image(systemName: "speaker.1.fill")
                }
                .frame(minWidth: 0, maxWidth: .infinity)
                .padding()
                .foregroundColor(.black)
                .background(Color.orange)
                .cornerRadius(10)
                .padding(.horizontal, 20)
                            Button(action: {
                    //soundbox start shuffle/next song
                    self.ble.sendData(data: "99".data(using: .utf8))

                }) {
                    Image(systemName: "shuffle")
                }
                .frame(minWidth: 0, maxWidth: .infinity)
                .padding()
                .foregroundColor(.black)
                .background(Color.orange)
                .cornerRadius(10)
                .padding(.horizontal, 5)
                Button(action: {
                    //soundbox volume up
                    self.ble.sendData(data: "98".data(using: .utf8))

                }) {
                    Image(systemName: "speaker.3.fill")
                }
                .frame(minWidth: 0, maxWidth: .infinity)
                .padding()
                .foregroundColor(.black)
                .background(Color.orange)
                .cornerRadius(10)
                .padding(.horizontal, 20)
            }
            .padding(.vertical, 10)
            
            ASCollectionView(data: dataExample, dataID: \.self) { item, _ in
                Color.blue
                    .animation(.default)
                    .overlay(Text("\(self.tools.get_name_from_key(key: item, dict_name: "key_to_label")!)"))
                    .onTapGesture {
                        //instead of playing the track, send the tracknumber via BLE to the Soundbox
                        self.ble.sendData(data: self.tools.get_name_from_key(key: item, dict_name: "key_to_number")!.data(using: .utf8))
                }
            .cornerRadius(10)
            }
            .layout {
                .grid(layoutMode: .adaptive(withMinItemSize: 100),
                    itemSize: .absolute(50))
            }
        }
    }
}

    
struct ContentView: View {
    //sound stuff
    var tracks: NSDictionary?
    var tools = Tools()
    //ble stuff
    //CBCentralManager reference should be a strong reference to the class as a member variable.
    //It cannot work as a local reference.
    @ObservedObject var ble = BLEService()

    init() {
        self.tracks = tools.read_dict(dict_name: "key_to_track")
        //start BLE module
        // important: to only find the appropriate ble device, we have to scan for the specific
        // service FFE0 (HM10 BLE module)
        self.ble.startCentralManager()
    }

    var body: some View {
        let keys = tracks!.map{$0.key}
        
        return NavigationView {
            
            //start ASCollectionView, convert all members of the keys array to String
            if self.ble.connected {
                CollectionViewConnected(ble: ble, dataExample: tools.conv_any_to_str(input: keys))
                .navigationBarTitle("Adam's Soundbox")
            }
            else {
                CollectionViewNotConnected(ble: ble, dataExample: tools.conv_any_to_str(input: keys))
                .navigationBarTitle("Adam's Soundbox")
            }
        }.navigationViewStyle(StackNavigationViewStyle())
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}




class Tools {
    var audio_player = AVAudioPlayer()
    
    func conv_any_to_str(input: [Any]) -> [String] {
        var output: [String] = []
        
        for n in 0..<input.count {
            output.append(String(describing: input[n]))
        }
        return output
    }
    
    func playTrack(key: String) {
        let path = Bundle.main.path(forResource:
            get_name_from_key(key: key, dict_name: "key_to_track"), ofType: ".mp3")
        let url = URL(fileURLWithPath: path!)
                
        do {
            audio_player = try AVAudioPlayer(contentsOf: url)
            audio_player.prepareToPlay()
            audio_player.play()
        }
        catch {
            print(error)
        }
    }
    
    func get_name_from_key(key: String, dict_name: String) -> String? {
        var filename: String?
        let track_dict = read_dict(dict_name: dict_name)
        
        filename = track_dict![key] as? String
        
        return filename
    }
    
    
    //read a dictionary file, where keys are translated to track filenames
    func read_dict(dict_name: String) -> NSDictionary? {
        var track_dictionary: NSDictionary?
        if let path = Bundle.main.path(forResource: dict_name, ofType: "plist") {
           track_dictionary = NSDictionary(contentsOfFile: path)
        }
        return track_dictionary
    }
    
}


//binding method:

//declare @state here, react to changed value here
//struct ContentView: View {
//    @State private var showingAddUser = false
//
//    var body: some View {
//        VStack {
//            // your code here
//        }
//    }
//    .sheet(isPresented: $showingAddUser) {
//        // show the add user view
//    }
//}
//
//and do a @binding here, change value here
//struct AddView: View {
//    @Binding var isPresented: Bool
//
//    var body: some View {
//        Button("Dismiss") {
//            self.isPresented = false
//        }
//    }
//}

