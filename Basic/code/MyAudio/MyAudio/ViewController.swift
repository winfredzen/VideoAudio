//
//  ViewController.swift
//  MyAudio
//
//  Created by 王振 on 2022/6/22.
//

import Cocoa

class ViewController: NSViewController {
    
    var recStatus: Bool = false
    
    var button: NSButton?
    var thread: Thread?

    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        
        self.view.setFrameSize(NSSize(width: 640, height: 480))
        
        button = NSButton(title: "开始录制", target: self, action: #selector(buttonAction(sender:)))
        button?.setFrameSize(NSSize(width: 100, height: 30))
        button?.setFrameOrigin(NSPoint(x: 0, y: 0))
        self.view.addSubview(button!)
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }
    
    @objc func buttonAction(sender: NSButton) {
        print("buttonAction")

        recStatus = !recStatus
        if recStatus {
            button?.title = "停止录制"
            thread = Thread.init(target: self, selector:#selector(start), object: nil)
            thread?.start()
        } else {
            button?.title = "开始录制"
            //停止录制
            set_status(0);
        }
    }
    
    @objc func start() {
        print("start thread")
        record_audio();
    }


}

