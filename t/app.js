import { pipeline, env } from 'https://cdn.jsdelivr.net/npm/@xenova/transformers@2.17.2';

env.allowLocalModels = false;
env.useBrowserCache = true;

let translator;
let book;
let rendition;
let quitNow;

quitNow=false;

const fileInput = document.getElementById('fileInput');
const viewer = document.getElementById('viewer');
const translationDiv = document.getElementById('translation');
const rightPane = document.getElementById('rightPane');
const statusDiv = document.getElementById('status');
const stopTranslation = document.getElementById("stopTranslation")
const startTranslation = document.getElementById("startTranslation")


async function initTranslator() {
  stopTranslation.style.display = "none";
  startTranslation.style.display = "none";
  statusDiv.innerText = "Loading translation model (~80MB)...";
  try {
    translator = await pipeline(
      'translation',
      'Xenova/opus-mt-de-en'
    );
    statusDiv.innerText = "Model loaded. Upload a German EPUB file.";
    // stopTranslation.style.display = "block";
    startTranslation.style.display = "block";

  } catch (err) {
    statusDiv.innerText = "Model loading failed: " + err;
  }
}

// async function translateText(text) {
//   if (!translator) return;
//   statusDiv.innerText = "Translating page...";
//   translationDiv.innerText = "Translating...";

//   try {
//     const output = await translator(text.slice(0, 2000), {
//       max_length: 512
//     });
//     translationDiv.innerText = output[0].translation_text;
//     statusDiv.innerText = "Translation complete.";
//   } catch (err) {
//     translationDiv.innerText = "Translation error: " + err;
//     statusDiv.innerText = "Translation failed.";
//   }
// }

async function translateText(text) {
  if (!translator) return;

  statusDiv.innerText = "Translating page...";
  translationDiv.innerText = "Translating...";

  try {
    // 1️⃣ Normalize whitespace
    text = text.replace(/\s+/g, " ").trim();

    // 2️⃣ Split into sentences (simple heuristic)
    const sentences = text.match(/[^.!?]+[.!?]+|[^.!?]+$/g);
    if (!sentences) {
      translationDiv.innerText = "No text to translate.";
      return;
    }


    

    // 3️⃣ Group sentences into safe chunks (~500 chars each)
    const chunks_ = [];
    let currentChunk = "";

    for (const sentence of sentences) {
      if ((currentChunk + sentence).length > 30) {
        chunks_.push(currentChunk);
        currentChunk = sentence;
      } else {
        currentChunk += sentence;
      }
    }

    if (currentChunk) {
      chunks_.push(currentChunk);
    }

    const chunks = chunks_.filter(str => str !== "");

    // 4️⃣ Translate chunk-by-chunk
    let finalTranslation = "";
    quitNow = false;
    console.log("About to translate, here are the chunks")
    console.log(chunks)
    translationDiv.innerText = chunks[0] + "..." + " 🧐";
    await new Promise(resolve => setTimeout(resolve, 0));

    for (let i = 0; i < chunks.length && !quitNow; i++) {
      statusDiv.innerText = `Translating chunk ${i + 1} of ${chunks.length}...`;

      const output = await translator(chunks[i]);
      // const output = chunks[i]; //await translator(chunks[i]);

      // finalTranslation += output[0].translation_text + " ";
      // finalTranslation += chunks[i] + " ";
      // finalTranslation += chunks[i] + "--" + output[0].translation_text + " ";
      finalTranslation += output[0].translation_text + " ";
      translationDiv.innerText = finalTranslation.trim() + (i<chunks.length ? "\n\n" + chunks[i+1] + "..." : " ") + " 🧐";
      console.log(finalTranslation.trim() + "  🧐");
      await new Promise(resolve => setTimeout(resolve, 0));
      // requestAnimationFrame(translateText);
    }

    // 5️⃣ Display result
    translationDiv.innerText = finalTranslation.trim(); //finalTranslation.trim();
    statusDiv.innerText = "Translation complete.";

  } catch (err) {
    translationDiv.innerText = "Translation error: " + err;
    statusDiv.innerText = "Translation failed.";
  }
}

fileInput.addEventListener('change', function (e) {
  const file = e.target.files[0];
  if (!file) return;

  statusDiv.innerText = "Preparing EPUB...";
  book = ePub(file);

  book.ready.then(async () => {
    statusDiv.innerText = "Rendering EPUB...";
    rendition = book.renderTo("viewer", {
      width: "100%",
      height: "100%",
      spread: "none",
      allowScriptedContent: true
    });

    await book.ready;
    await book.locations.generate(1600);
    return rendition.display();

  }).then(() => {
    statusDiv.innerText = "EPUB loaded.";

    // Attach keyboard navigation once iframe renders
    let keyboardAttached = false;

    rendition.on("rendered", (section, contents) => {
      if (keyboardAttached) return;

      const iframe = document.querySelector("#viewer iframe");
      if (!iframe) return;

      const iframeDoc = iframe.contentDocument;

      iframeDoc.addEventListener("keydown", handleKeyNavigation);
      document.addEventListener("keydown", handleKeyNavigation);

      keyboardAttached = true;
      window.document.addEventListener("keydown", handleKeyNavigation);

      attachSwipeHandlers(contents);

    });
    // rendition.on("rendered", (section, contents) => {
    //   attachSwipeHandlers(contents);
    // });

    // rendition.on("rendered", () => {
    // rendition.manager.container.addEventListener("keydown", (event) => {
    //   switch (event.key) {
    //     case "ArrowRight":
    //     case "ArrowDown":
    //     case "PageDown":
    //     case " ":
    //       statusDiv.innerText = "Next page";
    //       event.preventDefault();
    //       rendition.next();
    //       break;

    //     case "ArrowLeft":
    //     case "ArrowUp":
    //     case "PageUp":
    //       statusDiv.innerText = "Previous page";
    //       event.preventDefault();
    //       rendition.prev();
    //       break;
    //   }
    // });
  // });

  }).catch(err => {
    statusDiv.innerText = "EPUB failed: " + err;
  });

  // Translate when location changes

  window.addEventListener("keydown", async (event) => {
    // if (["input", "textarea"].includes(document.activeElement.tagName.toLowerCase())) return;

    if (event.key.toLowerCase() === "t") {
      // event.preventDefault();
      await triggerTranslation();
    }

    if (event.key.toLowerCase() === "q") {
      // event.preventDefault();
      quitNow = true;
    }

  });

  // document.addEventListener("keyup", () => triggerTranslation());
  // document.addEventListener("click", () => triggerTranslation());
  // document.addEventListener("keydown", () => triggerTranslation());
  // document.addEventListener("click", () => triggerTranslation());
  // const iframe = document.querySelector("#viewer iframe");
  // if (!iframe) return;
  // const iframeDoc = iframe.contentDocument;
  // iframeDoc.addEventListener("click", handleKeyNavigation);

});

// async function getCurrentPageText() {
//   const location = rendition.currentLocation();
//   console.log(location)
//   if (!location) return "";

//   const range = await book.getRange(
//     location.start.cfi,
//     location.end.cfi
//   );
//   console.log(range)
//   console.log(range.toString())

//   return range ? range.toString() : "";
// };
// async function getCurrentPageText() {
//   const contents = rendition.getContents();
//   if (!contents.length) return "";

//   const content = contents[0];
//   const doc = content.document;
//   const win = content.window;

//   const visibleWidth = win.innerWidth;

//   let text = "";

//   const walker = doc.createTreeWalker(
//     doc.body,
//     NodeFilter.SHOW_TEXT,
//     null,
//     false
//   );

//   while (walker.nextNode()) {
//     const node = walker.currentNode;
//     const range = doc.createRange();
//     range.selectNodeContents(node);

//     const rects = range.getClientRects();

//     for (let rect of rects) {
//       if (rect.left >= 0 && rect.right <= visibleWidth) {
//         text += node.textContent + " ";
//         break;
//       }
//     }
//   }

//   return text.trim();
// }

async function getCurrentPageText() {
  const location = rendition.currentLocation();
  if (!location) return "";

  const contents = rendition.getContents();
  if (!contents.length) return "";

  const content = contents[0];
  const doc = content.document;

  const startRange = content.range(location.start.cfi);
  const endRange   = content.range(location.end.cfi);

  if (!startRange || !endRange) return "";

  const range = doc.createRange();
  range.setStart(startRange.startContainer, startRange.startOffset);
  range.setEnd(endRange.endContainer, endRange.endOffset);

  return range.toString();
}

async function triggerTranslation() {
  statusDiv.innerText = "Translation triggered";
  setTimeout(async () => {
    const iframe = viewer.querySelector("iframe");
    if (!iframe || !iframe.contentDocument) return;

    // const text = iframe.contentDocument.body.innerText;
    // const text = rendition.getContents();
    // const location = rendition.currentLocation();
    // const cfi = location.start.cfi;
    // let text;
    // book.getRange(cfi).then(range => {
    //   const text = range.toString();
    //   console.log("Extracted text:");
    //   console.log(text);
    // });    
    // let text
    const text = await getCurrentPageText();
    statusDiv.innerText = text;
    console.log("Extracted text:");
    console.log(text);

    // if (moreWork) {
    //     requestAnimationFrame(step);
    // }
    // const text = iframe.contentWindow.getSelection().toString();

    if (text && text.trim().length > 10) {
      statusDiv.innerText = "Starting translate";
      // translateText('Ich bin Frau Rao. Und ich leben nach Hyderabad. Ich habe viel zeit.');
      // translateText('Ich glaube dass die wald ist gut.');
      await translateText(text);
      statusDiv.innerText = "Translated";
    }
    else
    {
      statusDiv.innerText = "No text extracted";  
    }
    
  }, 300);
}




initTranslator();



function handleKeyNavigation(event) {
  if (!rendition) return;

  const activeTag = document.activeElement.tagName.toLowerCase();
  if (activeTag === "input" || activeTag === "textarea") return;

  switch (event.key) {
    case "ArrowRight":
    case "ArrowDown":
    case "PageDown":
    case " ":
      event.preventDefault();
      rendition.next();
      break;

    case "ArrowLeft":
    case "ArrowUp":
    case "PageUp":
      event.preventDefault();
      rendition.prev();
      break;
  }
}

document.getElementById("nextPage").onclick = () => {
  rendition.next();
};

document.getElementById("prevPage").onclick = () => {
  rendition.prev();
};

let currentFontSize = 100;

function applyFontSize() {
  rendition.themes.fontSize(currentFontSize + "%");
  // Translation panel
  translationDiv.style.fontSize = currentFontSize + "%";
}

document.getElementById("increaseFont").onclick = () => {
  currentFontSize += 10;
  applyFontSize();
};

document.getElementById("decreaseFont").onclick = () => {
  currentFontSize = Math.max(60, currentFontSize - 10);
  applyFontSize();
};


startTranslation.onclick = async () => {
  stopTranslation.style.display = "block";
  startTranslation.style.display = "none";
  await triggerTranslation();

};

stopTranslation.onclick = () => {
  startTranslation.style.display = "block";
  stopTranslation.style.display = "none";

  quitNow = true;
};


document.getElementById("toggleTranslation").onclick = ()=> {

  if (rightPane.style.display === "none") {
    rightPane.style.display = "block";
  } else {
    rightPane.style.display = "none";
  }
  refreshLayout()
}

document.getElementById("refreshLayout").onclick = () => {
  refreshLayout()
};




function refreshLayout() {
  setTimeout(() => {
    rendition.resize();
  }, 50);
}
window.addEventListener("resize", refreshLayout);



function attachSwipeHandlers(contents) {
  const doc = contents.document;

  let startX = 0;
  let startY = 0;
  let isSwiping = false;

  doc.addEventListener("touchstart", (e) => {
    if (e.touches.length !== 1) return;

    startX = e.touches[0].clientX;
    startY = e.touches[0].clientY;
    isSwiping = true;
  });

  doc.addEventListener("touchend", (e) => {
    if (!isSwiping) return;

    const endX = e.changedTouches[0].clientX;
    const endY = e.changedTouches[0].clientY;

    const deltaX = endX - startX;
    const deltaY = endY - startY;

    const minSwipeDistance = 50;

    // Ensure horizontal swipe dominates
    if (Math.abs(deltaX) > minSwipeDistance &&
        Math.abs(deltaX) > Math.abs(deltaY)) {

      if (deltaX < 0) {
        rendition.next();
      } else {
        rendition.prev();
      }
    }

    isSwiping = false;
  });
}



    // if (event.key.toLowerCase() === "t") {
    //   // event.preventDefault();
      
    // }

    // if (event.key.toLowerCase() === "q") {
    //   // event.preventDefault();
    //   quitNow = true;
    // }

// document.addEventListener("keydown", (event) => {
//   // Ignore key presses if user is typing in a text field
//   const activeTag = document.activeElement.tagName.toLowerCase();
//   if (activeTag === "input" || activeTag === "textarea") return;

//   switch (event.key) {
//     case "ArrowRight":
//     case "ArrowDown":
//     case "PageDown":
//     case " ":
//       event.preventDefault();
//       rendition.next();
//       break;

//     case "ArrowLeft":
//     case "ArrowUp":
//     case "PageUp":
//       event.preventDefault();
//       rendition.prev();
//       break;
//   }
// });
