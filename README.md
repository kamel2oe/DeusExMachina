# DeusExMachina

> Deus ex machina (/ˌdeɪəs ɛks ˈmækɪnə, - ˈmɑːk-/ DAY-əs ex-MA(H)K-in-ə,[1] Latin: [ˈdɛ.ʊs ɛks ˈmaːkʰɪnaː]; plural: dei ex machina; English ‘god from the machine’) is a plot device whereby a seemingly unsolvable problem in a story is suddenly and abruptly resolved by an unexpected and unlikely occurrence.

My attempt at mixing machine learning with reading memory from a poker client, this doesn't use the crappy OCR ways of getting data but fetches straight from memory instantly with no delay allowing for faster equity calculations using the [Monte Carlo algorithm](https://en.wikipedia.org/wiki/Monte_Carlo_method).

Will use tensorflow, C++, imgui (GUI) and reverse engineering.

Platform(s) supported: Windows

Requirments:

[DirectX Software Development Kit](https://www.microsoft.com/en-gb/download/details.aspx?id=6812)

**Work in progress**

Features:

- [x] Reverse-engineer the client to find structures for retrieving information about a table.
- [x] Output information to a GUI using imgui
- [ ] Create a game manager to handle state.
- [x] Add equity calculation using [OMPEval](https://github.com/zekyll/OMPEval)
- [ ] Handle inputting information back into the client fold/call/bet
- [x] Support multiple tables

Screenshots:

![ss1](screenshots/1.png)
![ss2](screenshots/2.png)
