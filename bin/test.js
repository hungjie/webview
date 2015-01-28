$(document).ready(function() {
    try {
        jsQObject.signal.connect(slot);
        jsQObject.slotThatEmitsSignal();
    }
    catch(e) {
        alert(e);
    }
});

function slotfrom(object) {
    var objectString = object.sender + " has emited signal " + object.signalsEmited + " times.";
    alert(objectString);
}

function func() {
    try {
        var object = {intValue: 2};
        var returnedObject = jsQObject.slotThatReturns(object);
        alert(returnedObject.stringValue);
        dump_obj(jsQObject);
        //jsQObject.sendtojs.connect(slotfrom);
        //jsQObject.slotThatEmitsSignal();
    }
    catch(e){
        alert(e);
    }
}

function dump_obj(myObject) {
  var s = "";
  for (var property in myObject) {
   s = s + "\n "+property +": " + myObject[property] ;
  }
  alert(s);
}
