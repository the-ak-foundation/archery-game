# Image Tools

This folder contains small helper scripts for preparing images used in Markdown documentation.

## `py3`

`py3` can upscale small OLED screenshots or GIFs using nearest-neighbor scaling. This keeps pixel-style images sharp instead of blurry when they are shown larger in Markdown.

### Usage

```bash
resources/images/tools/py3 -t zoom -v 4 -i resources/images/screens/download.png -o resources/images/screens/download_x4.png
```

Arguments:

| Option | Meaning |
|---|---|
| `-t zoom` | Run the zoom/upscale task |
| `-v 4` | Scale factor, for example `4` means `4x` |
| `-i <path>` | Input image path |
| `-o <path>` | Output image path |

### Auto Output Name

If `-o` is not provided, the tool automatically writes the output next to the input file and adds `_x<scale>` before the extension.

```bash
resources/images/tools/py3 -t zoom -v 4 -i resources/images/screens/download.png
```

Output:

```text
resources/images/screens/download_x4.png
```

### GIF Example

```bash
resources/images/tools/py3 -t zoom -v 4 -i resources/images/screens/gif_archery_game_menu.gif
```

Output:

```text
resources/images/screens/gif_archery_game_menu_x4.gif
```

### Notes

- The tool uses Pillow if available.
- If Pillow is not available, it falls back to ImageMagick `convert`.
- Scaling uses nearest-neighbor mode, which is best for `128x64` OLED screenshots and pixel-style GIFs.
