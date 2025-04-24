from typing import List, Dict, Optional
import re

@dataclass
class LogEvent:
    trigger: Optional[str]
    buffer: List[str]
    start_state: Dict[str, str]  # snapshot of variable_map before the trigger

class LogParser:
    def __init__(self, known_triggers: List[str], known_variables: List[str]):
        self.known_triggers = known_triggers
        self.known_variables = known_variables
        self.variable_map: Dict[str, str] = {}
        self.events: List[LogEvent] = []
        self.current_buffer: List[str] = []
        self.current_trigger: Optional[str] = None
        self.start_state_snapshot: Dict[str, str] = {}

    def parse_line(self, line: str):
        content = line.strip().split(']', 1)[-1].strip()  # remove timestamp

        # Check for variable update
        for var in self.known_variables:
            if re.match(fr"^{re.escape(var)}\s*:\s*", content):
                key, value = content.split(':', 1)
                self.variable_map[key.strip()] = value.strip()
                return  # handled as variable only

        # Check for trigger
        if content in self.known_triggers:
            if self.current_trigger:
                # Save current event
                self.events.append(LogEvent(
                    trigger=self.current_trigger,
                    buffer=self.current_buffer,
                    start_state=self.start_state_snapshot.copy()
                ))

            # Start new buffer
            self.current_trigger = content
            self.current_buffer = [line.strip()]  # include the trigger line
            self.start_state_snapshot = self.variable_map.copy()
            return

        # If no trigger and no var, just add line to buffer (if active)
        if self.current_trigger:
            self.current_buffer.append(line.strip())

    def parse_log_file(self, filepath: str) -> List[LogEvent]:
        with open(filepath, 'r') as f:
            for line in f:
                self.parse_line(line)

        # Final buffer flush
        if self.current_trigger:
            self.events.append(LogEvent(
                trigger=self.current_trigger,
                buffer=self.current_buffer,
                start_state=self.start_state_snapshot.copy()
            ))

        return self.events

