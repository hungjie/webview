$(document).ready(function() {
    try {
        jsQObject.signal.connect(slot);
        jsQObject.slotThatEmitsSignal();
    }
    catch(e) {
        alert(e);
    }
});

function slot(object) {
    var objectString = object.sender + " has emited signal " + object.signalsEmited + " times.";
    alert(objectString);
}

function func() {
    try {
        var object = {intValue: 1};
        var returnedObject = jsQObject.slotThatReturns(object);
        alert(returnedObject.stringValue);
    }
    catch(e){
        alert(e);
    }
}
