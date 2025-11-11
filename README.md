# No `rm`

## The why

This whole project started when I tried installing a package using `cd ~ && make install`, only to realise one of the lines was `rm . -rf`. And just like that `$HOME` was gone.

`rm` is not a very good command for a number of reasons:

1. it completely deletes files (they can arguably be recovered but definitely not in all cases)
2. it has a few gotchas, such as interpreting files named `-r`, `-f`, etc, as options when running `rm *`
3. more importantly: despite its risks, it remains the most well known, easiest to use (just 2 letters) alternative

These are the same issues it has had since its birth, in *1971*. Those same issues were *reimplemented* by the GNU crew when they re-wrote UNIX as free soft.

The MAN page idiotically mentions my second issue above in the following terms:

```
To remove a file whose name starts with a '-', for example '-foo', use one of these commands:
              rm -- -foo
              rm ./-foo
```

One might wonder how long a fix for this bug would take, as opposed to writing those lines in the first place.

## The how

Instead of `rm`, I decided to use `gio trash` which only moves stuff to the trash folder without deleting anything.

This way, you get a second chance to check what was "deleted" and restore it as needed.

For this purpose, I wrote `main.c` to be a drop-in replacement of `rm`'s but using `gio trash` instead.

> I am not proficient in C and I'm sure my script could use a lot of polishing. But hey, at least I checked the `malloc` call for errors.

How to use it:

1. `gcc main.c`
2. `sudo mv /bin/rm /bin/no-rm` to prevent the normal `rm` binary from getting used
3. `sudo mv ./a.out /bin/rm` to replace `rm` with out binary

And now, whenever anyone calls `rm`, they're calling `gio trash` instead. Your files are now safe in the trash folder.

## Further considerations

To further remove (ha!) your habit of using `rm`, I suggest setting up some informative alias, such as: `rm="rm is disabled on this system, please use gio trash instead"`.

Then you can also create an alias for `gio trash` to make it 2 letters long again, such as `gio trash=tt`.
