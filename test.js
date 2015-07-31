var addon = require('./build/Release/addon');

addon.run(function(err, param) {
    console.log("run1 - ", param);
});

setInterval(function() {console.log(new Date);}, 5000);
