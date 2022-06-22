//
//  ViewController.swift
//  MyAudio
//
//  Created by 王振 on 2022/6/22.
//

import Cocoa

class ViewController: NSViewController {

    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        
        self.view.setFrameSize(NSSize(width: 640, height: 480))
        
        let button = NSButton(title: "Button", target: self, action: #selector(buttonAction(sender:)))
        button.setFrameSize(NSSize(width: 100, height: 30))
        button.setFrameOrigin(NSPoint(x: 0, y: 0))
        self.view.addSubview(button)
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }
    
    @objc func buttonAction(sender: NSButton) {
        print("buttonAction")
        //测试调用C函数方法
        haha()
        
    }


}

