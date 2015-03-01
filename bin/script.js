/*
 try{
 jsQObject.Sendtojs.connect(slotfrom);
 }catch(e){
 alert(e);
 }
 object["cur_index"] = 0;
 object["cur_input"] = "";
 object["time"] = 1000;
 object["input_array"] = jsQObject.get_search_input_array();
 object["input_id"] = jsQObject.get_search_input_id();
 */

/*********************************************************************************************************************
 //以下由参数传�??
 var main_script = [ {"func":"scroll","parms":{"id":"kw"}},{"func":"move","parms":{"id":"kw"}}
 , {"func":"lbclick","parms":{"id":"kw"}}
 , {"func":"timerinputvalue","parms":{"cur_input":"", "time":1000, "input_array":["L","O"], "input_id":"kw"}}
 , {"func":"move","parms":{"class":"toindex", "class_index":0 ,"offset":{"left":5, "top":5}}}
 , {"func":"mbclick","parms":{"class":"toindex", "class_index":0, "offset":{"left":5, "top":5}}}
 , {"func":"sleep", "parms":{"mtime":1000}}
 , {"func":"waitLoadFinished","parms":{"times":5, "tab_index":1}}, {"func":"switchtab","parms":{"index":1}}
 , {"func":"move", "parms":{"id":"kw"}}, {"func":"lbclick", "parms":{}}
 , {"func":"timerinputvalue", "parms":{"cur_input":"", "time":1000, "input_array":["L","O", "V", "E"], "input_id":"kw"}}
 , {"func":"move", "parms":{"id":"su"}}, {"func":"lbclick", "parms":{"id":"su", "offset":{"left":5, "top":5}}}
 , {"func":"waitLoadFinished","parms":{"times":10, "tab_index":1}}
 , {"func":"whilembroll", "parms":{"id":"su", "limit_times":10}}
 , {"func":"exit", "parms":{"ret":0, "code":""}}
 , {"func":"forfunc", "parms":{"limit_times":5, "cur_times":0, "end_status":false,"end_condition":"foundelementid", "end_parms":{"id":"","offset":{}}, "action_index":0, "actions":[{"func":"move", "parms":{"id_class":{"id":"mainsrp-pager", "class":"J_Ajax num icon-tag"}}}, {"func":"lbclick", "parms":{"id_class":{"id":"mainsrp-pager", "class":"J_Ajax num icon-tag"}}}]}}
 ];
 //***********************************************************************************************************************/
function dump_obj(myObject) {
    var s = "";
    for (var property in myObject) {
        s = s + "\n " + property + ": " + myObject[property];
    }
    alert(s);
}

//factory_action define below

function foundelementid(object) {
    this.object = object;
    this.action = function()
    {
        var e = findElementByParms(this.object);

        if (e === undefined || e === null)
        {
            var id = this.object.random_class_id.id;
            var ee = document.getElementById(id);
            var class_name = this.object.random_class_id.class;
            var res = ee.getElementsByClassName(class_name);
            var randomnum = 1;
            if (res.length > 1)
            {
                randomnum = res.length;
            }

            //alert(res.length);

            var index = parseInt((randomnum - 1) * Math.random());

            var index_res = res[index];

            //alert(index);

            var sy = getElementTop(index_res);
            var sx = getElementLeft(index_res);

            var parms = {"top": sy, "left": sx};
            
            jsQObject.randomoption(parms);

            return false;
        }

        //alert("found");
        return true;
    };
}

function forfunc(object) {
    this.object = object;
    this.action = function()
    {
        if (this.object.limit_times <= this.object.cur_times)
        {
            this.object.end_status = true;
            jsQObject.forfunc(this.object);
            return;
        }

        if (this.object.action_index >= this.object.actions.length)
        {
            this.object.action_index = 0;
            this.object.cur_times++;
        }

        if (this.object.action_index === 0)
        {
            var c = do_factory(this.object.end_condition, this.object.end_parms);
            if (c === undefined || c === null)
            {
                this.object.end_status = true;
                jsQObject.forfunc(this.object);
                return;
            }

            if (c.action() === true)
            {
                this.object.end_status = true;
                jsQObject.forfunc(this.object);
                return;
            }
        }

        var cur_index = this.object.action_index;
        this.object.action_index++;
        jsQObject.forfunc(this.object);

        var parms2 = this.object.actions[cur_index];
        
        var a = do_factory(parms2.func, parms2.parms);
        a.action();
    };
}

function lbclick(object)
{
    this.object = object;
    this.action = function()
    {
        //alert("test");
        //var left = this.object.left;
        //var top = this.object.top;

        var top = -1;
        var left = -1;

        var e = findElementByParms(this.object);

        if (e !== undefined && e !== null)
        {
            top = getElementTop(e);
            left = getElementLeft(e);
        }

        var offset_top = 0;
        var offset_left = 0;

        if (this.object.offset !== undefined)
        {
            offset_top = this.object.offset.top;
            offset_left = this.object.offset.left;
        }

        var parms = {"left": left + offset_left, "top": top + offset_top};

        jsQObject.lbclick(parms);
    };
}

function mbroll(object)
{
    this.object = object;
    this.action = function()
    {
        var top = -1;
        var left = -1;

        var e = document.getElementById(this.object.id);
        if (e !== undefined && e !== null)
        {
            top = getElementTop(e);
            left = getElementLeft(e);
        }

        var ch = this.object.ch;

        var parms = {"left": left, "top": top, "ch": ch};

        jsQObject.mbroll(parms);
    };
}

function whilembroll(object)
{
    this.object = object;
    this.action = function()
    {
        var top = -1;
        var left = -1;

        var e = document.getElementById(this.object.id);
        if (e !== undefined && e !== null)
        {
            top = getElementTop(e);
            left = getElementLeft(e);
        }

        var limit_times = this.object.limit_times;
        var parms = {"left": left, "top": top, "limit_times": limit_times};

        jsQObject.whileMBRoll(parms);
    };
}

function mbclick(object)
{
    this.object = object;
    this.action = function()
    {
        var top = -1;
        var left = -1;

        var e = findElementByParms(this.object);

        if (e !== undefined && e !== null)
        {
            top = getElementTop(e);
            left = getElementLeft(e);
        }

        var offset_top = 0;
        var offset_left = 0;

        if (this.object.offset !== undefined)
        {
            offset_top = this.object.offset.top;
            offset_left = this.object.offset.left;
        }
        var parms = {"left": left + offset_left, "top": top + offset_top};

        jsQObject.mbclick(parms);
    };
}

function move(object)
{
    this.object = object;
    this.action = function()
    {
        var e = findElementByParms(this.object);

        var top = getElementTop(e);
        var left = getElementLeft(e);

        var offset_top = 0;
        var offset_left = 0;

        if (this.object.offset !== undefined)
        {
            offset_top = this.object.offset.top;
            offset_left = this.object.offset.left;
        }
        var parms = {"left": left + offset_left, "top": top + offset_top};

        jsQObject.move(parms);
    };
}

function scroll(object)
{
    this.object = object;
    this.action = function()
    {
        var e = findElementByParms(this.object);//document.getElementById(this.object.id);
        var sy = getElementTop(e);
        var sx = getElementLeft(e);

        var offset_top = 0;
        var offset_left = 0;

        if (this.object.offset !== undefined)
        {
            offset_top = this.object.offset.top;
            offset_left = this.object.offset.left;
        }

        var parms = {"left": sx + offset_left, "top": sy + offset_top};
        
        jsQObject.scroll(parms);
    };
}

function randomscroll(object)
{
    this.object = object;
    this.action = function()
    {
        var randomoption = jsQObject.randomoption();      

        var parms = {"top": randomoption.top - 100, "left": randomoption.left - 100};
        
        jsQObject.scroll(parms);
    };
}

function randommove(object)
{
    this.object = object;
    this.action = function()
    {
        var randomoption = jsQObject.randomoption();
        return;

        var parms = {"top": randomoption.top + 10, "left": randomoption.left + 10};

        jsQObject.move(parms);
    };
}

function randommbclick(object)
{
    this.object = object;
    this.action = function()
    {
        var randomoption = jsQObject.randomoption();

        var parms = {"top": randomoption.top + 10, "left": randomoption.left + 10};
        
        jsQObject.mbclick(parms);
    };
}

function timerinputvalue(object)
{
    this.object = object;

    this.action = function() {
        jsQObject.timerInput(this.object);
    };
}

function waitLoadFinished(object)
{
    this.object = object;
    this.action = function() {
        jsQObject.waitLoadFinished(this.object);
    };
}

function switchtab(object)
{
    this.object = object;
    this.action = function() {
        var index = this.object.index;
        jsQObject.switchtab(index);
    };
}

function sleep(object)
{
    this.object = object;
    this.action = function() {
        var mtime = this.object.mtime;
        jsQObject.sleep(mtime);
    };
}

function exit(object)
{
    this.object = object;
    this.action = function() {
        jsQObject.exit(this.object);
    };
}

function do_factory(func, parms)
{
    var o;
    if (func === "lbclick")
    {
        o = new lbclick(parms);
    }
    else if (func === "move")
    {
        o = new move(parms);
    }
    else if (func === "scroll")
    {
        o = new scroll(parms);
    }
    else if (func === "timerinputvalue")
    {
        o = new timerinputvalue(parms);
    }
    else if (func === "switchtab")
    {
        o = new switchtab(parms);
    }
    else if (func === "sleep")
    {
        o = new sleep(parms);
    }
    else if (func === "waitLoadFinished")
    {
        o = new waitLoadFinished(parms);
    }
    else if (func === "mbclick")
    {
        o = new mbclick(parms);
    }
    else if (func === "mbroll")
    {
        o = new mbroll(parms);
    }
    else if (func === "whilembroll")
    {
        o = new whilembroll(parms);
    }
    else if (func === "exit")
    {
        o = new exit(parms);
    }
    else if (func === "forfunc")
    {
        o = new forfunc(parms);
    }
    else if (func === "foundelementid")
    {
        o = new foundelementid(parms);
    }
    else if (func === "randomscroll")
    {
        o = new randomscroll(parms);
    }
    else if (func === "randommove")
    {
        o = new randommove(parms);
    }
    else if (func === "randommbclick")
    {
        o = new randommbclick(parms);
    }

    //dump_obj(o);

    return o;
}

function factory(action, option)
{
    var o;

    if (main_script.length === 0 || main_script.length <= action)
        return o;

    var parms = main_script[action];

    if (option !== undefined)
    {
        parms.parms = option;
    }

    return do_factory(parms.func, parms.parms);
}

function factory_action(object) {
    //dump_obj(object);
    var step = object.signalsEmited;
    var option = object.option;

    var a = factory(step, option);

    if (a === null || a === undefined)
    {
        return;
    }

    a.action();
}

function init()
{
    jsQObject.Sendtojs.connect(factory_action);
}

function start()
{
    //alert(globalvar);
    //alert("start");
    // > a[href='http://big.taobao.com/?spm=1.7388829.a214tlt.19']
    /*
     var e = qt.jQuery("#OSC_Links");
     var top = e.offset().top;
     var left = e.offset().left;
     */

    //var e = document.getElementById("HomeJobPanel");
    /*
     var es = document.getElementsByClassName("pic-link");
     alert(es.length);
     alert(jsQObject.isLoadFinished());
     return;
     */

    var step = 0;
    var parms = {"signalsEmited": step};
    factory_action(parms);
}

function findElementByParms(parms) {
    var e;
    if (parms.id !== undefined)
    {
        e = document.getElementById(parms.id);
    }
    else if (parms.class !== undefined)
    {
        var es = document.getElementsByClassName(parms.class);
        if (parms.class_index !== undefined && parms.class_index < es.length)
        {
            e = es[parms.class_index];
        }
        else
        {
            e = es[0];
        }
    }
    else if (parms.id_class !== undefined)
    {
        if (parms.id_class.id !== undefined && parms.id_class.class !== undefined)
        {
            if (parms.id_class.class_index !== undefined)
            {
                e = getElementsByIdAndClass(parms.id_class.id, parms.id_class.class, parms.id_class.class_index);
            }
            else
            {
                e = getElementsByIdAndClass(parms.id_class.id, parms.id_class.class);
            }

        }
    }

    return e;
}

function getElementsByIdAndClass(id, class_name, class_index)
{
    var e = document.getElementById(id);
    var res = e.getElementsByClassName(class_name);

    if (res === undefined)
        return res;

    var index = class_index === undefined ? 0 : class_index;
    if (class_index === -1)
    {
        index = res.length - 1;
    }

    if (index < 0)
        index = 0;

    return res[index];
}

function getElementLeft(element)
{
    var actualLeft = element.offsetLeft;
    var current = element.offsetParent;
    while (current !== null) {
        actualLeft += current.offsetLeft;
        current = current.offsetParent;
    }
    return actualLeft;
}

function getElementTop(element)
{
    var actualTop = element.offsetTop;
    var current = element.offsetParent;
    while (current !== null) {
        actualTop += current.offsetTop;
        current = current.offsetParent;
    }
    return actualTop;
}

function getElementViewLeft(element) {
    var actualLeft = element.offsetLeft;
    var current = element.offsetParent;
    while (current !== null) {
        actualLeft += current.offsetLeft;
        current = current.offsetParent;
    }
    if (document.compatMode === "BackCompat") {
        var elementScrollLeft = document.body.scrollLeft;
    } else {
        var elementScrollLeft = document.documentElement.scrollLeft;
    }

    return actualLeft - elementScrollLeft;
}

function getElementViewTop(element) {
    var actualTop = element.offsetTop;
    var current = element.offsetParent;
    while (current !== null) {
        actualTop += current.offsetTop;
        current = current.offsetParent;
    }
    if (document.compatMode === "BackCompat") {
        var elementScrollTop = document.body.scrollTop;
    } else {
        var elementScrollTop = document.documentElement.scrollTop;
    }
    return actualTop - elementScrollTop;
}
