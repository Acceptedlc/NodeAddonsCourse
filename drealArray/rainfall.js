var rainfall = require("./build/Release/rainfall");

var makeup = function(max) {
    return Math.round(max * Math.random() * 100)/100;
}

var locations = []
for (var i = 0; i < 10; i++ ) {
    var loc = {
        latitude: makeup(180),
        longitude: makeup(180),
        samples : [
            {date: "2015-07-20", rainfall: makeup(3)},
            {date: "2015-07-21", rainfall: makeup(3)},
            {date: "2015-07-22", rainfall: makeup(3)},
            {date: "2015-07-23", rainfall: makeup(3)}
        ]
    }
    locations.push(loc);
}

var results = rainfall.calculate_results(locations);
console.log(JSON.stringify(results,null,2));
