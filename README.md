# .:[ PinVMShield ]:.

PinVMShield is a tool to fool (malware) binaries and avoid VMs-sandbox common detection techniques. It uses dynamic binary instrumentation (DBI) techniques to perform the tricks. Namely, it has been developed with the Pin DBI framework.

This project has been developed with Visual Studio 2008 v9.0.30729.1 SP, configured for Pin DBI (see http://www.pintool.org for getting Pin). If you want to compile it by yourself, you may need some tuning in the project configuration. Also, other programs are required to be executed. See Requeriments for more details.

PinVMShield can be easily integrated with sandbox environments for automation of malware analysis tasks with DBI capabilities. Namely, we provide an integration for Cuckoo Sandbox. You can find it in the [`sandbox_integration`](sandbox_integration) folder. The file inside such a folder must be replace the one located at `$CUCKOOPATH/analyzer/windows/modules/packages`.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

## Requeriments
- Visual Studio 2008 (for compilation)
- Pin 2.13-61206-msvc9 (for running, not assure to be reliable at newer versions)

## Version History

- Version 1.1 (May, 2014)
- Version 1.0 (January, 2014)
- Alpha release

## License

Licensed under the [GNU GPLv3](LICENSE) license.

## Authory

This program has been developed by Ricardo J. Rodríguez, researcher at Technical University of Madrid, Spain, and Iñaki Rodríguez-Gastón, security researcher at MLW.RE. You can look forward more information about them or their work on their own webpages, http://www.ricardojrodriguez.es and http://virtualminds.es/. 
