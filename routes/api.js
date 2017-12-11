var express = require('express');
var storage = require('node-persist');
var router = express.Router();

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
        storage.setItemSync('currentPattern',id);
        console.log("Pattern changed to " + id);
    }
    if (start) {
        [hours, min] = start.split(":",2);
        storage.setItemSync('startTime',new Date(2016,1,1,hours,min,0,0));
        console.log("Start time changed to " + storage.getItemSync('startTime'));
    }
    if (stop) {
        [hours, min] = stop.split(":",2);
        storage.setItemSync('stopTime',new Date(2016,1,1,hours,min,0,0));
        console.log("Stop time changed to " + storage.getItemSync('stopTime'));
    }
    if (color1) {
        storage.setItemSync('color1',parseInt(color1));
    }
    if (color2) {
        storage.setItemSync('color2',parseInt(color2));
    }
    if (brightness) {
        storage.setItemSync('brightness',parseInt(brightness));
    }
    result.brightness = storage.getItemSync('brightness');
    result.color1 = storage.getItemSync('color1');
    result.color2 = storage.getItemSync('color2');

    var currentDate = new Date();
    var currentTime = new Date(2016,1,1,currentDate.getHours(), currentDate.getMinutes(), currentDate.getSeconds(), currentDate.getMilliseconds());
    var tStart = new Date(storage.getItemSync('startTime'));
    var tStop = new Date(storage.getItemSync('stopTime'));

    //If the start time is less than the stop time, then we want the current time to be inside them. If not, we want the current time to be outside them.
    if( ((tStart < tStop) && (currentTime > tStart && currentTime < tStop)) ||
        ((tStop < tStart) && (currentTime > tStart || currentTime < tStop)) )
        {
            result.pattern = parseInt(storage.getItemSync('currentPattern'));
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
