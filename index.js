const Input = require('./build/Release/sendinput.node');

function SendInput(inputs){
    if(!Array.isArray(inputs))
        inputs = [inputs];
    let arr = [];
    for(let inp of inputs){
        if(typeof inp != "object")
            throw new Error("Expecting array of objects");
        if(!Number.isInteger(inp.type) || inp.type < 0 || inp.type > 2)
            throw new Error("Expecting type to be an integer from 0 to 2");
        if(!Number.isInteger(inp.val))
            throw new Error("Expecting val to be an integer")
        switch(inp.type){
            case 0:
                arr.push(Input.CreateKBDInpVKey(inp.val, !!inp.up));
            break;
            case 1:
                let val = inp.val;
                let extended = false;
                if(val >> 8 & 0xFF == 0xe0)
                    extended = true;
                arr.push(Input.CreateKBDInpScanCode(inp.val & 0xFF, !!inp.up, extended));
            break;
            case 2:
                arr.push(Input.CreateKBDInpUnicode(inp.val, !!inp.up));
            break;
        }
    }
    return Input.SendInput(arr);
}
module.exports = {SendInput}
