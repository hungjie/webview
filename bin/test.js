/*
try{
	jsQObject.Sendtojs.connect(slotfrom);
}catch(e){
	alert(e);
}
*/

var main_script = new array("first", "second");

function func() {
    try {
        //var object = {intValue: 2};
        //var returnedObject = jsQObject.slotThatReturns(object);
        //alert(returnedObject.stringValue);
        dump_obj(jsQObject);
        jsQObject.Sendtojs.connect(slotfrom);
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

//factory_action define the flow

function lbclick(object)
{
	this.object = object;
	this.action = function()
	{
		//alert("test");
		var left = this.object.left;
		var top = this.object.top;
		
		var parms = {"left":left, "top":top};
		
		jsQObject.lbclick(parms);
	}
}

function move(object)
{
}

function scroll(object)
{
}

function inputvalue(object)
{
	this.object = object;
	
	this.action = function(){
		//var value = object["value"];
		var value = new Array("L","你");
		var e = document.getElementById("kw");
		var input = "";
		
		for(var i=0; i < value.length; i++){
			input = input + value[i];
			e.value = input;
			
			//sleep(5000);
		}
	}
}

function switchtab(object)
{
}

function sleep(d){
	
}

function factory(action)
{
}

function factory_action(object) {
    //var objectString = object.sender + " has emited signal " + object.signalsEmited + " times.";
    //alert(objectString);
	//dump_obj(object);
	
	if(object.signalsEmited == 1)
	{
		var top = object.top;
		var left = object.left;
		
		var parms = {"top":top, "left":left+50};
		
		jsQObject.move(parms);
	}
	else if(object.signalsEmited == 2)
	{
		var c = new lbclick(object);
		c.action();
	}
	else if(object.signalsEmited == 3)
	{
		var i = new inputvalue(object);
		i.action();
	}
}

function start()
{
    //alert("start");
    // > a[href='http://big.taobao.com/?spm=1.7388829.a214tlt.19']
    /*
	var e = qt.jQuery("#OSC_Links");
    var top = e.offset().top;
    var left = e.offset().left;
	*/
	
	//var e = document.getElementById("HomeJobPanel");
	var e = document.getElementById("kw");
	var top = getElementTop(e);
	var left = getElementLeft(e);

    alert("top:" + top + ", left:" + left);

    var parms = {"top":top, "left":left};
	
	jsQObject.Sendtojs.connect(factory_action);
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
