# Watch the compile_commands.json file and, whenever it changes, remove any moc_*.cpp files
#
# Requirements: pip install watchdog

import sys
import os
import json
import time
from time import sleep
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler


class FileChangeHandler(FileSystemEventHandler):
    @staticmethod
    def on_modified(event):
        if not event.is_directory and (os.path.basename(event.src_path) == "compile_commands.json"):
            sleep(0.5)  # Delay for 500ms
            FileChangeHandler.filter_moc_entries(event.src_path)

    @staticmethod
    def filter_moc_entries(file_path):
        with open(file_path, 'r') as f:
            data = json.load(f)

        # Filter out moc entries
        data = [entry for entry in data if not os.path.basename(
            entry['file']).startswith('moc_')]

        # Write back filtered data
        with open(file_path, 'w') as f:
            json.dump(data, f, indent=2)


if __name__ == "__main__":
    path = 'compile_commands' if len(sys.argv) < 2 else sys.argv[1]
    event_handler = FileChangeHandler()
    observer = Observer()
    observer.schedule(event_handler, path, recursive=False)
    observer.start()
    print("Watching for changes to compile_commands.json...")
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()
