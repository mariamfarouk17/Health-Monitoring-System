"""
serial_receiver.py
Reusable serial parsing logic. Updated to include activity status
(0 = idle, 1 = walking, 2 = running) as sent by the STM32 firmware.
"""

PACKET_FIELDS = {
    "HR":     ("heart_rate",      int),
    "STEPS":  ("steps",           int),
    "DIST":   ("distance",        float),
    "CAL":    ("calories",        int),
    "BAT":    ("battery",         int),
    "STATUS": ("activity_status", int),
}


def parse_packet(line: str):
    if not line:
        return None

    parts = line.split(";")
    if len(parts) != len(PACKET_FIELDS):
        return None

    result = {}
    for part in parts:
        if "=" not in part:
            return None

        key, _, value_str = part.partition("=")
        key = key.strip()
        value_str = value_str.strip()

        if key not in PACKET_FIELDS:
            return None

        field_name, cast_func = PACKET_FIELDS[key]

        try:
            result[field_name] = cast_func(value_str)
        except ValueError:
            return None

    expected_field_names = {field_name for field_name, _ in PACKET_FIELDS.values()}
    if set(result.keys()) != expected_field_names:
        return None

    return result