# -*- coding:utf-8-unix; mode:ruby; -*-
require File.expand_path('../lib/revdev/version', __FILE__)

Gem::Specification.new do |g|
  g.name	= "revdev"
  g.version	= Revdev::VERSION
  g.authors	= ["Keiichiro Ui"]
  g.email	= ["keiichiro.ui@gmail.com"]
  g.summary	= "ruby binding to handling event devices."
  g.description	= "revdev is a ruby binding to handling event devices."
  g.homepage	= "https://rubygems.org/gems/revdev"
  g.files	= `git ls-files`.split($\)
  # g.files	= Dir.glob 'lib/**/*.rb'
  # g.files	+= Dir.glob 'ext/**/*.{rb,c,h}'
  g.executables	= g.files.grep(%r{^bin/}).map{ |f| File.basename(f) }
  g.test_files	= g.files.grep(%r{^(test|spec|features)/})
  g.require_paths	= ["lib"]
  g.extensions	= Dir.glob 'ext/**/extconf.rb'
  g.add_development_dependency 'rake'
  g.add_development_dependency 'bundler'
end
