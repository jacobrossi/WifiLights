var express = require('express');
var router = express.Router();
var leds = require('../leds');

/* GET Read current pattern */
router.get('/:id?', function(req, res, next) {
    var pattern = req.params.id;
    var start = req.query.start;
    var stop = req.query.stop;
    var color1 = req.query.color1;
    var color2 = req.query.color2;
    var brightness = req.query.brightness;

    if (pattern) {
        leds.pattern = pattern;
    }
    if (start) {
        [hours, min] = start.split(":",2);
        leds.startTime = new Date(2016,1,1,hours,min,0,0);
    }
    if (stop) {
        [hours, min] = stop.split(":",2);
        leds.stopTime = new Date(2016,1,1,hours,min,0,0);
    }
    if (color1) {
        leds.color1 = parseInt(color1);
    }
    if (color2) {
        leds.color2 = parseInt(color2);
    }
    if (brightness) {
        leds.brightness = parseInt(brightness);
    }

    var result = {pattern: leds.pattern, color1: leds.color1, color2: leds.color2, brightness: leds.brightness}; //We make a new object for the result because we don't need to send all the values like start/stop time
    var currentDate = new Date();
    var currentTime = new Date(2016,1,1,currentDate.getHours(), currentDate.getMinutes(), currentDate.getSeconds(), currentDate.getMilliseconds());
    var tStart = new Date(leds.startTime);
    var tStop = new Date(leds.stopTime);

    //If the start time is less than the stop time, then we want the current time to be inside them. If not, we want the current time to be outside them.
    if( ((tStart < tStop) && (currentTime > tStart && currentTime < tStop)) ||
        ((tStop < tStart) && (currentTime > tStart || currentTime < tStop)) )
        {
            result.pattern = parseInt(leds.pattern);
            console.log("Inside operating hours.");
            console.log("START:" + tStart.getTime());
            console.log("STOP:" + tStop.getTime());
            console.log("NOW:"+currentTime.getTime());
            console.log("Current pattern: " + result.pattern);
    } else {
            result.pattern = 0;
            console.log("Outside operating hours.");
            console.log("START:" + tStart.getTime());
            console.log("STOP:" + tStop.getTime());
            console.log("NOW:"+currentTime.getTime());
    }
    res.send(JSON.stringify(result))
});

module.exports = router;
