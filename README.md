# Temblor

Temblor is a user interface for processing seismic waveform data.  It is geared towards processing data collected by local and regional seismic networks.  In particular, I am interested in asking the question of how to incorporate machine learning into an analyst' workflow.  And I refuse to attempt to answer that question with AQMS's Jiggle because some things belong in the past.

*Warning* I'm still screwing around with the idea of making a UI.  I have no deadlines or timelines so this project may stall indefinitely.

## Directory Structure

In an attempt to decouple Temblor's functionality from its user interface the major code segregration is split between 

   1. lib - handles the business logic, i.e., here is where the Temblor actually performs operations on the data models.
   2. gui - provides a view of the business logic, i.e., here is where you interact with the data models on the screen.

The reason for distinction is two-fold.  First, you can take the temblor core business logic and potentially tackle other interesting problems.  Second, we can, and do, write unit tests for the core business logic.

Since the GUI is very specific it is doubtful whether it will ever be suitable for a library.  Consequently, the include splits

   1. include/temblor/library - the includes for the core business logic.
   2. include/temblor/ui - the includes for the user interfaces.

The namespacing then

   - Temblor::Library::Utilities
   - Temblor::Library::DataReaders

TemblorUI::Applications
TemblorUI::
