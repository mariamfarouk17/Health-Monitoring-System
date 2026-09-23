/**
 * script.js
 *
 * Real-time WebSocket connection to the Python backend at
 * ws://localhost:8765. Now also handles the "activity_status" field
 * (0 = idle, 1 = walking, 2 = running).
 */

const WEBSOCKET_URL = "ws://localhost:8765";
const RECONNECT_DELAY_MS = 3000;

// Maps the numeric STM32 activity status to display info.
const ACTIVITY_STATES = {
  0: { state: "idle",    label: "Idle" },
  1: { state: "walking", label: "Walking" },
  2: { state: "running", label: "Running" },
};

let socket = null;

function updateDashboard(data) {
  document.getElementById("heart-rate").textContent = data.heart_rate;
  document.getElementById("steps").textContent = data.steps;
  document.getElementById("distance").textContent = data.distance;
  document.getElementById("calories").textContent = data.calories;
  document.getElementById("battery").textContent = data.battery;

  updateBatteryGauge(data.battery);
  updateActivityStatus(data.activity_status);
  updateStepsRing(data.steps); // NEW
}

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
 * Updates the activity badge based on the numeric status (0/1/2).
 * If the value is missing or unrecognized, falls back to "idle"
 * instead of crashing, since older packets without STATUS= are
 * still valid for the other fields.
 */
function updateActivityStatus(activityStatus) {
  const badgeEl = document.getElementById("activity-status");
  const labelEl = badgeEl.querySelector(".activity-label");

  const info = ACTIVITY_STATES[activityStatus] || ACTIVITY_STATES[0];

  badgeEl.dataset.state = info.state;
  labelEl.textContent = info.label;
}

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
      console.warn("[WebSocket] Received malformed JSON, ignoring:", event.data);
      return;
    }

    updateDashboard(data);
  };

  socket.onerror = (event) => {
    console.error("[WebSocket] Connection error:", event);
  };

  socket.onclose = () => {
    console.warn("[WebSocket] Disconnected from backend.");
    setConnectionStatus(false);
    setTimeout(connectWebSocket, RECONNECT_DELAY_MS);
  };
}

document.addEventListener("DOMContentLoaded", () => {
  setConnectionStatus(false);
  connectWebSocket();
});



// Assumption: daily step goal not provided by backend, defaulting to 10,000.
const DAILY_STEP_GOAL = 10000;
const RING_CIRCUMFERENCE = 326.73; // 2 * PI * r(52), must match CSS

function updateStepsRing(steps) {
  const ringEl = document.getElementById("steps-ring-progress");
  const percent = Math.max(0, Math.min(1, steps / DAILY_STEP_GOAL));
  const offset = RING_CIRCUMFERENCE * (1 - percent);
  ringEl.style.strokeDashoffset = offset;
}