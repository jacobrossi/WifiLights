var express = require('express');
//var api = require('/routes/api.js');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  var startTime = 
  res.render('index', { 
    title: 'Christmas Lights', 
    currentPattern: require.main.currentPattern,
    brightness: require.main.brightness,
    startTime: (require.main.startTime.getHours()=="0"?"00":require.main.startTime.getHours()) + ":" + require.main.startTime.getMinutes(),
    stopTime: (require.main.stopTime.getHours()=="0"?"00":require.main.stopTime.getHours()) + ":" + require.main.stopTime.getMinutes(),
    color1: "#" + require.main.color1.toString(16).padStart(6,"0"),
    color2: "#" + require.main.color2.toString(16).padStart(6,"0"),
    helpers: {
      checked: function(id) {
        if (id==require.main.currentPattern) {
          return 'checked';
        }
      },
      notchecked: function(id) {
        if (id!=require.main.currentPattern) {
          return 'checked';
        }
      }
    }});
});

module.exports = router;
