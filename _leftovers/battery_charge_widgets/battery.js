var self = {
  element: $('.bh-batteryWidget'),
  options: {
    colour: "#transparent",
    widgetIdentifier: "batteryWidget",
    title: "Battery Info",
    batteryLevelLabel: 'Battery Level',
    batteryVoltageLabel: 'Voltage',
    sourceCode: 'VEHCL',
    sourcePk: null
  },

  batteryValue: 80,
  // create func
  _createElements: function() 
  {
  
  // BATTERY
  
    // create battery container elements
    self.element.append('<div class="container-div batteryContainer"></div>');
    
    // add wigget batter title
    self.element.find('.batteryContainer').append('<div class="widget-title"><b></b></div>');

    // set battery title
    self.setBatteryTitle();

    // add battery level indicator
    self.element.find('.batteryContainer').append('<div class="battery"><div class="level"></div></div>');

    // append battery value label
    self.element.find('.batteryContainer').find('.battery').after('<div class="value-label"><b></b></div>')

    // set battery Value
    self.setBatteryValue(self.batteryValue);
    
    // VOLTAGE
    
    
  },
  
  // set battery title
  setBatteryTitle: function() 
  {
    self.element.find('.batteryContainer').find('b').text(self.options.batteryLevelLabel);
  },
  
  // set battery label
  setBatteryValue: function(value) 
  {
    self.element.find('.batteryContainer').find('.value-label').find('b').text(value + ' %');    
    var w = value + '%';
    
    self.element.find('.batteryContainer').find('.level').css('width',w);    
  }  
};


self._createElements();

//<script src="libs/battery.js"></script>
