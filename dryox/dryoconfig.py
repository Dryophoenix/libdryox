import tomllib
from dryox.dryoinit import dryoinit, Dryox_XDG_Dir
from dryox.dryolog import dryolog, LogLevel

conffile: str = dryoinit("config.toml", "libdryox", Dryox_XDG_Dir.DROYX_XDG_CONFIG)

