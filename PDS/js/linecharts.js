 // Loading local JSON file 
function loadJSON(callback) {   
    var xobj = new XMLHttpRequest();
        xobj.overrideMimeType("application/json");
    xobj.open('GET', 'log.json', true); 
    xobj.onreadystatechange = function () {
          if (xobj.readyState == 4 && xobj.status == 200) {
            callback(xobj.responseText);
          }
    };
    xobj.send(null);  
 }

 Array.prototype.mapProperty = function(property) {
 	var array = []
 	this.forEach(function(object){
 		if (object.hasOwnProperty(property)) {
 			array.push(object[property]);
 		}
 	});
 	return array;
 }
 
window.onload = function(){

	loadJSON(function(response) {
  // Parse JSON string into object
    
    data = JSON.parse(response);

// Sender Window Graph
$(function () {
  	var dataChart = [];
  	var x = data.mapProperty('SenderTime');
  	var y = data.mapProperty('SenderWin');

  	for (var i in x){
  		dataChart.push([Number(x[i]), Number(y[i])]);
  	}
  	dataChart.splice((dataChart.length - 1), 1);
        $('#sender').highcharts({
            chart: {
                zoomType: 'x'
            },
            title: {
                text: 'Sender Window'
            },
            subtitle: {
                text: document.ontouchstart === undefined ?
                        'Click and drag in the plot area to zoom in' : 'Pinch the chart to zoom in'
            },
            xAxis: {
                title: {
                	text: 'Seconds'
                }
            },
            yAxis: {
                title: {
                    text: 'Bytes'
                }
            },
            legend: {
                enabled: false
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: {
                            x1: 0,
                            y1: 1,
                            x2: 0,
                            y2: 1
                        },
                        stops: [
                            [0, Highcharts.getOptions().colors[0]],
                            [1, Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                        ]
                    },
                    marker: {
                    	enabled: true,
                        radius: 5
                    },
                    lineWidth: 1,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                }
            },

            series: [{
                type: 'area',
                name: 'Bytes',
                data: dataChart   
            }]
        });
});

// Receiver Window Graph
$(function () {
  	var dataChart = [];
  	var x = data.mapProperty('ReceiverTime');
  	var y = data.mapProperty('RecWin');

  	for (var i in x){
  		dataChart.push([Number(x[i]), Number(y[i])]);
  	}
  	dataChart.splice((dataChart.length - 1), 1);
        $('#receiver').highcharts({
            chart: {
                zoomType: 'x'
            },
            title: {
                text: 'Receiver Window'
            },
            subtitle: {
                text: document.ontouchstart === undefined ?
                        'Click and drag in the plot area to zoom in' : 'Pinch the chart to zoom in'
            },
            xAxis: {
                title: {
                	text: 'Seconds'
                }
            },
            yAxis: {
                title: {
                    text: 'Bytes'
                }
            },
            legend: {
                enabled: false
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: {
                            x1: 0,
                            y1: 1,
                            x2: 0,
                            y2: 1
                        },
                        stops: [
                            [0, Highcharts.getOptions().colors[0]],
                            [1, Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                        ]
                    },
                    marker: {
                    	enabled: true,
                        radius: 5
                    },
                    lineWidth: 1,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                }
            },

            series: [{
                type: 'area',
                name: 'Bytes',
                data: dataChart   
            }]
        });
});

// Speed Graph
$(function () {
  	var dataChart = [];
  	var dataChart2 = [];
  	var x1 = data.mapProperty('Time');
  	var y1 = data.mapProperty('BytesRec');

  	var x2 = data.mapProperty('TimeSen');
  	var y2 = data.mapProperty('BytesSen');

  	for (var i in x1){
  		dataChart.push([Number(x1[i]), Number(y1[i])]);
  	}
  	for (var j in x2){
  		dataChart2.push([Number(x2[j]), Number(y2[j])]);
  	}
  	dataChart.splice((dataChart.length - 1), 1);
  	dataChart2.splice((dataChart2.length - 1), 1);

        $('#speed').highcharts({
            chart: {
                zoomType: 'x'
            },
            title: {
                text: 'TCP speed (Measurements for interval 0.01 s)'
            },
            subtitle: {
                text: document.ontouchstart === undefined ?
                        'Click and drag in the plot area to zoom in' : 'Pinch the chart to zoom in'
            },
            xAxis: {
                title: {
                	text: 'Seconds'
                }
            },
            yAxis: {
                title: {
                    text: 'Bytes'
                }
            },
            legend: {
                enabled: true
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: {
                            x1: 0,
                            y1: 1,
                            x2: 0,
                            y2: 1
                        },
                        stops: [
                            [0, Highcharts.getOptions().colors[0]],
                            [1, Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                        ]
                    },
                    marker: {
                    	enabled: true,
                        radius: 5
                    },
                    lineWidth: 1,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                }
            },

            series: [{
                type: 'area',
                name: 'Bytes receiver',
                data: dataChart   
            }, {
            	type: 'area',
            	name: 'Bytes sender',
            	data: dataChart2 
            }]
        });
});

// Round trip time graph
$(function () {
  	var dataChart = [];
  	var x = data.mapProperty('Sequence');
  	var y = data.mapProperty('RTT');

  	for (var i in x){
  		dataChart.push([Number(x[i]), Number(y[i])]);
  	}
  	dataChart.splice((dataChart.length - 1), 1);
        $('#roundtrip').highcharts({
            chart: {
                zoomType: 'x'
            },
            title: {
                text: 'Round trip time'
            },
            subtitle: {
                text: document.ontouchstart === undefined ?
                        'Click and drag in the plot area to zoom in' : 'Pinch the chart to zoom in'
            },
            xAxis: {
                title: {
                	text: 'Sequence numbers'
                }
            },
            yAxis: {
                title: {
                    text: 'ms'
                }
            },
            legend: {
                enabled: false
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: {
                            x1: 0,
                            y1: 1,
                            x2: 0,
                            y2: 1
                        },
                        stops: [
                            [0, Highcharts.getOptions().colors[0]],
                            [1, Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                        ]
                    },
                    marker: {
                    	enabled: true,
                        radius: 5
                    },
                    lineWidth: 1,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                }
            },

            series: [{
                type: 'area',
                name: 'ms',
                data: dataChart   
            }]
        });
}); 

// Slow Start Receiver graph
$(function () {
  	var dataChart = [];
  	var x = data.mapProperty('TimeSS');
  	var y = data.mapProperty('SequenceSS');

  	for (var i in x){
  		dataChart.push([Number(x[i]), Number(y[i])]);
  	}
  	dataChart.splice((dataChart.length - 1), 1);
        $('#slowstartRec').highcharts({
            chart: {
                zoomType: 'x'
            },
            title: {
                text: 'Slow start (receiver)'
            },
            subtitle: {
                text: document.ontouchstart === undefined ?
                        'Click and drag in the plot area to zoom in' : 'Pinch the chart to zoom in'
            },
            xAxis: {
                title: {
                	text: 'Seconds'
                }
            },
            yAxis: {
                title: {
                    text: 'Sequence numbers'
                }
            },
            legend: {
                enabled: false
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: {
                            x1: 0,
                            y1: 1,
                            x2: 0,
                            y2: 1
                        },
                        stops: [
                            [0, Highcharts.getOptions().colors[0]],
                            [1, Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                        ]
                    },
                    marker: {
                    	enabled: true,
                        radius: 5
                    },
                    lineWidth: 1,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                }
            },

            series: [{
                type: 'area',
                name: 'Sequence number',
                data: dataChart   
            }]
        });
});

// Slow Start Sender graph
$(function () {
  	var dataChart = [];
  	var x = data.mapProperty('TimeSSsen');
  	var y = data.mapProperty('SequenceSSsen');

  	for (var i in x){
  		dataChart.push([Number(x[i]), Number(y[i])]);
  	}
  	dataChart.splice((dataChart.length - 1), 1);
        $('#slowstartSen').highcharts({
            chart: {
                zoomType: 'x'
            },
            title: {
                text: 'Slow start (sender)'
            },
            subtitle: {
                text: document.ontouchstart === undefined ?
                        'Click and drag in the plot area to zoom in' : 'Pinch the chart to zoom in'
            },
            xAxis: {
                title: {
                	text: 'Seconds'
                }
            },
            yAxis: {
                title: {
                    text: 'Sequence numbers'
                }
            },
            legend: {
                enabled: false
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: {
                            x1: 0,
                            y1: 1,
                            x2: 0,
                            y2: 1
                        },
                        stops: [
                            [0, Highcharts.getOptions().colors[0]],
                            [1, Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                        ]
                    },
                    marker: {
                    	enabled: true,
                        radius: 5
                    },
                    lineWidth: 1,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                }
            },

            series: [{
                type: 'area',
                name: 'Sequence number',
                data: dataChart   
            }]
        });
});

});

} 


