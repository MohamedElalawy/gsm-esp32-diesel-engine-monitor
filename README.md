# Diesel Sentinel 🚛📡  
> Remote Diesel Engine Monitoring System using ESP32 & SIM800L

**Diesel Sentinel** is an ESP32-based IoT telemetry node for **remote monitoring** of diesel engines. It tracks **engine working hours**, **temperature**, and **oil pressure**, then sends this data to **Google Sheets** via **GSM (SIM800L)** — with no need for Wi-Fi or cloud platforms.

---

## 🔧 Features
- 📈 Logs **engine work hours**, **temperature**, and **oil pressure**
- ☁️ Sends data to **Google Sheets** using HTTP POST over GSM
- 🛠️ Fully offline — operates without Wi-Fi or external cloud services
- ⏱️ Configurable update interval (e.g., every 5 minutes)
- 🔋 Compatible with 12V/24V engine battery systems

---

## 🧩 Hardware Overview

| Component             | Description                        |
|----------------------|------------------------------------|
| ESP32                | Main microcontroller               |
| SIM800L              | GSM module for HTTP communication  |
| Temperature Sensor   | Analog or digital sensor           |
| Oil Pressure Sensor  | Analog sensor (voltage output)     |
| Engine Status Input  | Digital input from engine system   |
| Power Supply         | Buck converter for 5V system power |

---

## ⚙️ System Architecture

```plaintext
+------------------+         +------------------+         +-------------------+
|  Temp Sensor     | ----->  |                  |         |                   |
|  Oil Pressure    | ----->  |     ESP32        | ----->  |   SIM800L GSM     | ---> Internet
|  Engine Status   | ----->  |                  |         |                   |
+------------------+         +------------------+         +-------------------+
```

---

## 🔥 Data Sent to Google Sheets

| Timestamp        | Work Hours | Temperature (°C) | Oil Pressure (bar) |
| ---------------- | ---------- | ---------------- | ------------------ |
| 2025-05-31 10:05 | 12.75      | 78.6             | 3.7                |

Data is sent in JSON format like:

```json
{
  "workHours": "12.75",
  "temperature": "78.6",
  "oilPressure": "3.7"
}
```

---

## 🧪 Setup Instructions

### 1. Google Sheets & Apps Script

* Create a new Google Sheet with columns:

  * `Timestamp`, `WorkHours`, `Temperature`, `OilPressure`
* Go to **Extensions > Apps Script**, and paste this code:

```javascript
function doPost(e) {
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Sheet1");
  var data = JSON.parse(e.postData.contents);
  sheet.appendRow([
    new Date(),
    data.workHours,
    data.temperature,
    data.oilPressure
  ]);
  return ContentService.createTextOutput("Success");
}
```