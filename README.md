Clipboard
=========
This NPAPI plugin allows access to the system clipboard.

Methods
-------

### `get()`

Get the contents of the clipboard, it takes an optional parameter with the name of the
format you want, the default is `text/plain`.

*Note: currently only text is supported.*

### `set()`

Set the contents of the clipboard, it takes the first parameter of type
dependant on the format you want it to be saved on the clipboard and an
optional format name, the default is `text/plain`.

*Note: currently only text is supported.*

### `available()`

Check if the clipboard contains data, it takes an optional parameter with the
name of the format you want to check for, the default is `text/plain`.

*Note: currently only text is supported.*

### `clear()`

Clear the contents of the clipboard.

Example
=======
To get access to the API you have to add an `<embed>` tag for the plugin to
work.

```html
<embed id="clipboard" width="0" height="0" type="application/x-clipboard">
```

In case it's not working, try adding the following code as early as possible.

```javascript
navigator.plugins.refresh(true);
```

Then you can access the methods to manipulate the clipboard on the `<embed>`
tag itself.

```javascript
function clipboard() {
  return document.getElementById("clipboard");
}

clipboard().set("hue")
console.log(clipboard().get()) // => "hue"
```

Also make sure the `<embed>` tag is not hidden, styling it to be `visibility:
hidden` or `display: none` will cause the browser to unload the plugin.

As an alternative you can use `position: absolute; top: -5px; left: -5px`.
