const Input = require('./build/Release/sendinput.node');
let inp = Input.CreateKBDInpScanCode(0x9, false);
let inp2 = Input.CreateKBDInpScanCode(0x9, true);
setTimeout(()=>{
    let i = 0;
    let iv = setInterval(()=>{
        Input.SendInput([inp])
        i++;
        if(i > 30){
            clearInterval(iv);
            Input.SendInput([inp2])
            console.log("sent", i)
        }
    }, 100)
}, 5000)

//console.log('addon', Input.SendInput([inp]));
