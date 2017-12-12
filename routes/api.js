var express = require('express');
var router = express.Router();

require.main.currentPattern = "1";
require.main.startTime = new Date(2016,1,1,16,30,0,0);
require.main.stopTime = new Date(2016,1,1,0,15,0,0);
require.main.color1 = 255; //Blue
require.main.color2 = 65280; //Green

/* GET Read current pattern */
router.get('/:id?', function(req, res, next) {
    var id = req.params.id;
    var start = req.query.start;
    var stop = req.query.stop;
    var color1 = req.query.color1;
    var color2 = req.query.color2;
    var brightness = req.query.brightness;
    var result = new Object();


    if (id) {
        require.main.currentPattern = id;
        console.log("Pattern changed to " + id);
    }
    if (start) {
        [hours, min] = start.split(":",2);
        require.main.startTime = new Date(2016,1,1,hours,min,0,0);
        console.log("Start time changed to " + require.main.startTime);
    }
    if (stop) {
        [hours, min] = stop.split(":",2);
        require.main.stopTime = new Date(2016,1,1,hours,min,0,0);
        console.log("Stop time changed to " + require.main.stopTime);
    }
    if (color1) {
        require.main.color1 = parseInt(color1);
    }
    if (color2) {
        require.main.color2 = parseInt(color2);
    }
    if (brightness) {
        require.main.brightness = parseInt(brightness);
    }
    result.brightness = require.main.brightness;
    result.color1 = require.main.color1;
    result.color2 = require.main.color2;

    var currentDate = new Date();
    var currentTime = new Date(2016,1,1,currentDate.getHours(), currentDate.getMinutes(), currentDate.getSeconds(), currentDate.getMilliseconds());
    //If the start time is less than the stop time, then we want the current time to be inside them. If not, we want the current time to be outside them.
    if( ((require.main.startTime < require.main.stopTime) && (currentTime > require.main.startTime && currentTime < require.main.stopTime)) || 
        ((require.main.stopTime < require.main.startTime) && (currentTime > require.main.startTime || currentTime < require.main.stopTime)) )
        {
            result.pattern = parseInt(require.main.currentPattern);
            console.log("Inside operating hours.");
            console.log("START:" + require.main.startTime.getTime());
            console.log("STOP:" + require.main.stopTime.getTime());
            console.log("NOW:"+currentTime.getTime());
            console.log("Current pattern: " + result.pattern);
    } else {
            result.pattern = 0;
            console.log("Outside operating hours.");
            console.log("START:" + require.main.startTime.getTime());
            console.log("STOP:" + require.main.stopTime.getTime());
            console.log("NOW:"+currentTime.getTime());
    }
    res.send(JSON.stringify(result))
});

module.exports = router;
