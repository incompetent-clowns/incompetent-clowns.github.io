# Project T

[Project T](https://incompetent-clowns.github.io/t) is a barebones Epub file reader, with side-by-side on-device translation. 

To run locally, on localhost:8000, use ```python -m http.server 8000```

## Current features
* Core: loads an epub file, and shows its translation
* Can remember the location of the last book you read (and also the font sizes)
* Can select which model is used for translation (have tested with German → English, German → French etc.)
* The UI allows one to hide the translation panel, and the top bar.
* Zero ads/bloat. 

## Wish-list
* Resize panels
* Integrate with a local LLM (if Ollama is installed, for instance)
* Add standard e-reader features
	* Navigation bar
	* A back button to return after pressing a link
	* Fix the page swipe action
	* Allow highlighted text to be translated
* See if there's a text to speech engine
* Persistence
	* Across device sync
	* Potentially keeping books in cache
	* Sharing current location with other people