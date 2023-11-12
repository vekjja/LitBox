void initializeWebServer() {
  wifi.setConnectSubroutine([]() { testMatrix(); });
  wifi.enableMDNS("spectral-analyzer");

  wifi.webserver()->on("/", HTTP_GET, []() {
    wifi.webserver()->send(200, "text/html", indexHTML);
  });

  wifi.webserver()->on("/set/sensitivity", HTTP_GET, []() {
    if (wifi.webserver()->hasArg("value")) {
      int newSensitivity = wifi.webserver()->arg("value").toInt();
      setSensitivity(newSensitivity);
      wifi.webserver()->send(200, "text/plain",
                             "Sensitivity set to: " + String(sensitivity));
    } else {
      wifi.webserver()->send(400, "text/plain", "Missing sensitivity value");
    }
  });

  wifi.webserver()->on("/set/visualization", HTTP_GET, []() {
    if (wifi.webserver()->hasArg("mode")) {
      int newMode = wifi.webserver()->arg("mode").toInt();
      setVisualization(newMode);
      wifi.webserver()->send(200, "text/plain",
                             "Visualization set to: " + String(visualization));
    } else {
      wifi.webserver()->send(400, "text/plain", "Missing visualization mode");
    }
  });

  wifi.APToClientMode();
}