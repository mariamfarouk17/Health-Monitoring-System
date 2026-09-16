/**
 * script.js
 *
 * STEP 5: Replaces STEP 4's mock data with a real WebSocket connection
 * to the Python backend at ws://localhost:8765.
 *
 * updateDashboard(data) and updateBatteryGauge(percentage) are UNCHANGED
 * from STEP 4 — only the data source feeding them is different now.
 */

// ---------------------------------------------------------------
// CONFIGURATION
// ---------------------------------------------------------------
const WEBSOCKET_URL = "ws://localhost:8765";
const RECONNECT_DELAY_MS = 3000; // retry connecting every 3s if disconnected

let socket = null;

/**
 * Writes a health-data object into the dashboard's DOM elements.
 * Same function as STEP 4 — now called from the WebSocket onmessage
 * handler instead of from mock data.
 */
function updateDashboard(data) {
  document.getElementById("heart-rate").textContent = data.heart_rate;
  document.getElementById("steps").textContent = data.steps;
  document.getElementById("distance").textContent = data.distance;
  document.getElementById("calories").textContent = data.calories;
  document.getElementById("battery").textContent = data.battery;

  updateBatteryGauge(data.battery);
}

/**
 * Updates the visual battery bar width and color based on percentage.
 * Unchanged from STEP 4.
 */
function updateBatteryGauge(percentage) {
  const fillEl = document.getElementById("battery-fill");

  const clamped = Math.max(0, Math.min(100, percentage));
  fillEl.style.width = clamped + "%";

  let color = "var(--battery-good)";
  if (clamped <= 20) {
    color = "var(--battery-low)";
  } else if (clamped <= 50) {
    color = "var(--battery-mid)";
  }

  fillEl.style.background = color;
}

/**
 * Sets the connection status indicator.
 * STEP 5: now driven by real WebSocket onopen/onclose/onerror events.
 */
function setConnectionStatus(isConnected) {
  const statusEl = document.getElementById("connection-status");
  const labelEl = statusEl.querySelector(".connection-label");

  if (isConnected) {
    statusEl.dataset.state = "connected";
    labelEl.textContent = "Connected";
  } else {
    statusEl.dataset.state = "disconnected";
    labelEl.textContent = "Disconnected";
  }
}

/**
 * Opens (or re-opens) the WebSocket connection to the Python backend
 * and wires up all the event handlers.
 */
function connectWebSocket() {
  socket = new WebSocket(WEBSOCKET_URL);

  socket.onopen = () => {
    console.log("[WebSocket] Connected to backend at", WEBSOCKET_URL);
    setConnectionStatus(true);
  };

  socket.onmessage = (event) => {
    let data;

    try {
      data = JSON.parse(event.data);
    } catch (err) {
      // Malformed JSON received - log and skip, do not crash the page
      console.warn("[WebSocket] Received malformed JSON, ignoring:", event.data);
      return;
    }

    updateDashboard(data);
  };

  socket.onerror = (event) => {
    // Errors are usually followed by onclose, so the status update
    // itself happens there. This just gives a clear console message.
    console.error("[WebSocket] Connection error:", event);
  };

  socket.onclose = () => {
    console.warn("[WebSocket] Disconnected from backend.");
    setConnectionStatus(false);

    // Automatically try to reconnect after a short delay, so the
    // dashboard recovers on its own once the backend is available again.
    setTimeout(connectWebSocket, RECONNECT_DELAY_MS);
  };
}

// ---------------------------------------------------------------
// Start the connection once the page has loaded.
// STEP 4's mock data and setInterval() demo loop have been removed.
// ---------------------------------------------------------------
document.addEventListener("DOMContentLoaded", () => {
  setConnectionStatus(false); // starts as "Disconnected" until onopen fires
  connectWebSocket();
});