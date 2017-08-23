//rainfall.js
var rainfall = require("./build/Release/rainfall");
var location = {
    latitude : 40.71, longitude : -74.01,
       samples : [
          { date : "2015-06-07", rainfall : 2.1 },
          { date : "2015-06-14", rainfall : 0.5}, 
          { date : "2015-06-21", rainfall : 1.5}, 
          { date : "2015-06-28", rainfall : 1.3}, 
          { date : "2015-07-05", rainfall : 0.9}
       ] };

var avg = rainfall.avg_rainfall(location)
console.log("Average rain fall = " + avg + "cm");

var data = rainfall.data_rainfall(location);

console.log("Mean = " + data.mean)
console.log("Median = " + data.median);
console.log("Standard Deviation = " + data.standard_deviation);
console.log("N = " + data.n);