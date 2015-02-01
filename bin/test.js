qt.jQuery(document).ready(function() {
    //alert("hello");
    try {
        //jsQObject.sendtojs.connect(slotfrom);
        //jsQObject.slotThatEmitsSignal();
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
        //var object = {intValue: 2};
        //var returnedObject = jsQObject.slotThatReturns(object);
        //alert(returnedObject.stringValue);
        dump_obj(jsQObject);
        jsQObject.sendtojs.connect(slotfrom);
        jsQObject.slotThatEmitsSignal();
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

function testpos()
{
    alert("start");
    // > a[href='http://big.taobao.com/?spm=1.7388829.a214tlt.19']
    var e = qt.jQuery("#OSC_Links");
    var top = e.offset().top;
    var left = e.offset().left;

    alert("top:" + top + ", left:" + left);

    var parms = {"top":top, "left":left};

    jsQObject.scroll(parms);
}

function getElementLeft(element)
{
    var actualLeft = element.offsetLeft;
    var current = element.offsetParent;
    while (current !== null){
    　　actualLeft += current.offsetLeft;
    　　current = current.offsetParent;
    }
    return actualLeft;
}

function getElementTop(element)
{
    var actualTop = element.offsetTop;
    var current = element.offsetParent;
    while (current !== null){
    　　actualTop += current.offsetTop;
    　　current = current.offsetParent;
    }
    return actualTop;
}

function getElementViewLeft(element){
    var actualLeft = element.offsetLeft;
    var current = element.offsetParent;
    while (current !== null){
    　　actualLeft += current.offsetLeft;
    　　current = current.offsetParent;
    }
    if (document.compatMode == "BackCompat"){
    　　var elementScrollLeft=document.body.scrollLeft;
    } else {
    　　var elementScrollLeft=document.documentElement.scrollLeft;
    }

    return actualLeft-elementScrollLeft;
}
function getElementViewTop(element){
    var actualTop = element.offsetTop;
    var current = element.offsetParent;
    while (current !== null){
    　　actualTop += current. offsetTop;
    　　current = current.offsetParent;
    }
     if (document.compatMode == "BackCompat"){
    　　var elementScrollTop=document.body.scrollTop;
    } else {
    　　var elementScrollTop=document.documentElement.scrollTop;
    }
    return actualTop-elementScrollTop;
}
