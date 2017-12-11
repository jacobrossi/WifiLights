function changePattern(pattern)
{
    var req = new XMLHttpRequest();
    req.open('GET', '/api/' + pattern);
    req.send();
    if (pattern!==0)
        lastPattern = pattern;
}
function changeBrightness(brightness)
{
    var req = new XMLHttpRequest();
    req.open('GET', '/api/?brightness=' + brightness);
    req.send();
}
function changeStartTime(time)
{
    var req = new XMLHttpRequest();
    req.open('GET', '/api/?start=' + time);
    req.send();
}
function changeStopTime(time)
{
    var req = new XMLHttpRequest();
    req.open('GET', '/api/?stop=' + time);
    req.send();
}
function changeColor1(color)
{
    var req = new XMLHttpRequest();
    req.open('GET', '/api/?color1=' + parseInt(color.substr(1),16));
    req.send();
}
function changeColor2(color)
{
    var req = new XMLHttpRequest();
    req.open('GET', '/api/?color2=' + parseInt(color.substr(1),16));
    req.send();
}

// iOS Safari screwed us on :active, so we'll work around that with script...ewwww
document.addEventListener('touchstart', function(){}, true);
document.addEventListener('touchstart', function(e){if(e.touches.length>1) e.preventDefault()}, true);

function changePage(page)
{
    if(page==1) {
        document.querySelector('.panels').classList.remove('page2');
    }else{
        document.querySelector('.panels').classList.add('page2');
    }
}