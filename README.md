# QML greetd dm

Base on [qt-session-lock](https://github.com/waycrate/qt-session-lock) and greetd, take a lot of reference from qtgreetd

## Example config

```toml
[terminal]
# The VT to run the greeter on. Can be "next", "current" or a number
# designating the VT.
vt = 1

# The default session, also known as the greeter.
[default_session]

# `agreety` is the bundled agetty/login-lookalike. You can replace `/bin/sh`
# with whatever you want started, such as `sway`.
command = "river -c waycratedm"

# The user to run the command as. The privileges this user must have depends
# on the greeter. A graphical greeter may for example require the user to be
# in the `video` group.
user = "greeter"

```

## View
![show](./images/show.png)
