These scripts enable and disable the collection of event tracing events from a running instance of the program. Generally, you won't need to use them.

Trace logging was useful for certain types of debugging, such as when the event processing logic within JargonPlayer was causing playback hitches across multiple windows.

To use trace event logging, you'll need the Windows Performance Toolkit:
https://docs.microsoft.com/en-us/windows-hardware/test/wpt/

Code within JargonPlayer fires custom trace events. Run the start script to begin collecting the events, then run the stop script to dump them to an ETL file.
The ETL file can then be opened with the Windows Performance Analyzer for inspection.
