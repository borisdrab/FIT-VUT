from collections import UserDict

class TimeSeriesDB(UserDict):
    """
    A class for storing time-stamped data linked to specific keys.
    """

    def __setitem__ (self, key, time_value_pair):
        """Adds a (timestamp, value) pair to the key."""
        timestamp, val = time_value_pair

        if key not in self.data:
            self.data[key] = [] 
            
        # Ensure that the new timestamp is in ascending order
        last_timestamp = self.data[key][-1][0] if self.data[key] else None
        if self.data[key] and self.data[key][-1][0] > timestamp:
            raise ValueError("Timestamp must be postive and in ascending order.")

        self.data[key].append((timestamp, val))
        
    def __getitem__ (self, query):
        """Retrieves the value for a key or (key, timestamp)."""

        # Handle (key, timestamp) queries
        if isinstance(query, tuple):
            key, requested_timestamp = query

            if key not in self.data:
                raise KeyError(f"Key '{key}' doesn't exist !")

            time_series = self.data[key]

            # Search for the closest timestamp that is <= the requested timestamp
            for stored_timestamp, stored_value in reversed(time_series):
                if stored_timestamp <= requested_timestamp:
                    return stored_value
            
            raise KeyError(f"Key '{key}' doesn't have a value for time {requested_timestamp}!")

        if query not in self.data:
            raise KeyError(f"Key '{query}' doesn't exist!")
        return self.data[query][-1][1]  # Most recent value
