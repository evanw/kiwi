#[macro_use]
extern crate clap;
extern crate failure;
#[macro_use]
extern crate failure_derive;
#[macro_use]
extern crate nom;
extern crate heck;

mod parser;
mod types;
mod errors;
mod keywords;

use std::path::{Path, PathBuf};
use clap::{App, Arg};
use types::{Config, FileDescriptor};
use failure::ResultExt;
use errors::Error;

fn run() -> Result<(), ::failure::Error> {
    let matches = App::new(crate_name!())
        .about(crate_description!())
        .author(crate_authors!("\n"))
        .version(crate_version!())
        .arg(
            Arg::with_name("OUTPUT")
                .required(false)
                .long("output")
                .short("o")
                .takes_value(true)
                .help("Generated file name, defaults to INPUT with 'rs' extension")
                .validator(|x| extension_matches(x, "rs")),
        )
        .arg(
            Arg::with_name("OUTPUT_DIR")
                .required(false)
                .long("output_directory")
                .short("d")
                .takes_value(true)
                .help("Output directory of generated code"),
        )
        .arg(
            Arg::with_name("INCLUDE_PATH")
                .required(false)
                .long("include")
                .short("I")
                .takes_value(true)
                .help("Path to search for imported protobufs"),
        )
        .arg(
            Arg::with_name("SINGLE_MOD")
                .required(false)
                .long("single-mod")
                .short("s")
                .help("Omit generation of modules for each package when there is only one package"),
        )
        .arg(
            Arg::with_name("NO_OUTPUT")
                .required(false)
                .long("no-output")
                .short("n")
                .help(
                    "Show enums and messages in this .proto file, including those imported. \
                     No code generated",
                ),
        )
        .arg(
            Arg::with_name("INCLUDE_CAPNP")
                .required(false)
                .long("include-capnp")
                .short("c")
                .help(
                    "Include canpnp output"
                ),
        )
        .arg(
            Arg::with_name("INPUT_WHITELIST")
                .required(false)
                .multiple(false)
                .long("whitelist")
                .short("W")
                .takes_value(true)
                .help("A .proto file reflecting a subset of same-name fields in the INPUT protos, to count as non-deprecated")
                .validator(|x| extension_matches(x, "kiwi-whitelist")),
        )
        .arg(
            Arg::with_name("INPUT")
                .multiple(true)
                .help("The .proto files used to generate quick-protobuf code")
                .validator(|x| extension_matches(x, "kiwi")),
        )
        .get_matches();

    let in_files = path_vec(values_t!(matches, "INPUT", String));
    if in_files.is_empty() {
        return Err(Error::NoKiwi.into());
    }

    for f in &in_files {
        if !f.exists() {
            return Err(Error::InputFile(format!("{}", f.display())).into());
        }
    }

    let mut include_path = path_vec(values_t!(matches, "INCLUDE_PATH", String));
    let default = PathBuf::from(".");
    if include_path.is_empty() || !include_path.contains(&default) {
        include_path.push(default);
    }

    if in_files.len() > 1 && matches.value_of("OUTPUT").is_some() {
        return Err(Error::OutputMultipleInputs.into());
    }

    for in_file in in_files {
        let mut out_file = in_file.with_extension("rs");

        if let Some(ofile) = matches.value_of("OUTPUT") {
            out_file = PathBuf::from(ofile);
        } else if let Some(dir) = matches.value_of("OUTPUT_DIR") {
            let mut directory = PathBuf::from(dir);
            if !directory.is_dir() {
                return Err(Error::OutputDirectory(format!("{}", directory.display())).into());
            }
            directory.push(out_file.file_name().unwrap());
            out_file = directory;
        }

        let config = Config {
            in_file: in_file,
            whitelist_file: None,
            out_file: out_file,
            include_capnp: matches.is_present("INCLUDE_CAPNP"),
            single_module: matches.is_present("SINGLE_MOD"),
            import_search_path: include_path.clone(),
            no_output: matches.is_present("NO_OUTPUT"),
        };

        FileDescriptor::write_proto(&config).context(format!(
            "Could not convert {} into {}",
            config.in_file.display(),
            config.out_file.display()
        ))?;
    }
    Ok(())
}

fn extension_matches<P: AsRef<Path>>(path: P, expected: &str) -> std::result::Result<(), String> {
    match path.as_ref().extension() {
        Some(x) if x == expected => Ok(()),
        Some(x) => Err(format!(
            "Expected path with extension '{}', not: '{}'",
            expected,
            x.to_string_lossy()
        )),
        None => Err(format!("Expected path with extension '{}'", expected)),
    }
}

fn path_vec(maybe_vec: std::result::Result<Vec<String>, clap::Error>) -> Vec<PathBuf> {
    maybe_vec
        .unwrap_or_else(|_| Vec::new())
        .iter()
        .map(|s| s.into())
        .collect()
}

fn main() {
    ::std::process::exit({
        if let Err(e) = run() {
            eprintln!("kw-rs fatal error");
            for e in e.causes() {
                eprintln!("  - {}", e);
            }
            1
        } else {
            0
        }
    });
}
