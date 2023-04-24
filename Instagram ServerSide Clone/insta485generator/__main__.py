"""Build static HTML site from directory of HTML templates and plain files."""

import os
from os import path
import json
import pathlib
import shutil
import sys
from json.decoder import JSONDecodeError
import click
import jinja2


@click.command()
@click.argument(
    'input_dir', metavar="INPUT_DIR", type=click.Path(exists=True)
)
@click.option('-o', '--output', metavar='PATH', help="Output directory.")
@click.option('-v', '--verbose', is_flag=True, help="Print more output.")
def main(input_dir, output, verbose):
    """Templated static website generator."""
    input_dir = pathlib.Path(input_dir)  # convert str to Path object

    if output is None:
        output_dir = input_dir/"html"
    else:
        output_dir = pathlib.Path(output)

    # check if output_dir exists
    if path.isdir(output_dir):
        print("Output directory already exists")
        sys.exit(1)

    # check if static_directory exists
    static_dir = None
    if path.isdir(input_dir/"static"):
        static_dir = input_dir/"static"
        shutil.copytree(static_dir, output_dir)
        if verbose:
            print("Copied ", str(static_dir), " -> ", str(output_dir))
    else:
        os.mkdir(output_dir)

    try:
        with open(input_dir/"config.json", encoding="utf-8") as f_file:
            data = json.load(f_file)
    except (FileNotFoundError, TypeError, JSONDecodeError, ValueError):
        print("json error")
        sys.exit(1)

    # access each dictionary entry in config file
    for entry in data:
        url = entry["url"].lstrip("/")
        template_path = entry["template"].lstrip("/")
        context = entry["context"]
        output_path = output_dir/url/"index.html"

        if not path.isdir(output_dir/url):
            os.makedirs(output_dir/url)

        try:
            template_env = jinja2.Environment(
                loader=jinja2.FileSystemLoader(str(input_dir/"templates")),
                autoescape=jinja2.select_autoescape(['html', 'xml']),
            )
            template = template_env.get_template(template_path)
            with open(output_path, 'w', encoding="utf-8") as f_file:
                f_file.write(template.render(context))
        except (jinja2.TemplateNotFound, jinja2.TemplateError,
                jinja2.TemplateSyntaxError, jinja2.UndefinedError,
                jinja2.TemplateAssertionError):
            print("jinja2 error")
            sys.exit(1)

        if verbose:
            print("Rendered ", str(template_path), " -> ", str(output_path))


if __name__ == "__main__":
    main()
