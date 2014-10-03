Pebble.addEventListener("ready",
  function(e) {
    console.log("JavaScript app ready and running!");
  }
);
Pebble.addEventListener("appmessage",
  function(e) {
    console.log("Received message: " + e.payload);
    Pebble.showSimpleNotificationOnPebble("Titel", "Aangekomen");
  }
);