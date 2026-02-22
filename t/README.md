# Project T

[Project T](https://incompetent-clowns.github.io/t) is a barebones Epub file reader, with side-by-side on-device translation, developed by [Incompetent Clowns](https://incompetent-clowns.github.io/). 

To run locally, on localhost:8000, use ```python -m http.server 8000```

## Current features
* Core: loads an epub file, and shows its translation
* Can remember the location of the last book you read (and also the font sizes)
* Can select which model is used for translation (have tested with German → English, German → French etc.)
* The UI allows one to hide the translation panel, and the top bar.
* Zero ads/bloat. 

## Wish-list
* Basic UI features
	* Resize panels
	* Dark mode (the iFrame messes things up with in-built dark mode)
	* The dropdown for translation text is wonky; needs to be fixed
* Integrate with a local LLM (if Ollama is installed, for instance)
* Add standard e-reader features
	* Navigation bar
	* A back button to return after pressing a link
	* Fix the page swipe action
	* Allow highlighted text to be translated
* See if there's a text to speech engine
* Persistence
	* Remember which language translation model was last selected
	* Across device sync
	* Potentially keeping books in cache
	* Sharing current location with other people


## License
Assume it is GPL for now.

## Story

* Wanted to learn German. Couldn't find a utility/application as basic as this. 
* Started with ChatGPT generated code (Feb 20-23, 2026) and after a lot of debugging, it now appears to be usable.
* Huge thanks to the community behind the ePub javascript library (for actually rendering the ePub files) and Hugging Face transformer models (for translation). 