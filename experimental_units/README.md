# Experimental Material
Research project: Evaluate Docker's impact on real time system performance using [OpenDaVinci](https://github.com/se-research/OpenDaVINCI/).

### Code
The evaluation is done through two different types of code:

* [TimeTriggerCode](https://github.com/docker-rt-research/experimental-material/experimental_units/tree/master/TimeTriggerCode)
  * Code runs as slave to the [OpenDaVinci](https://github.com/se-research/OpenDaVINCI/) component `supercomponent`.
  * Is used for capturing data to understand Docker's impact on the scheduling precision of the real-time application

* [TimeTriggerCodeIO](https://github.com/docker-rt-research/experimental-material/experimental_units/tree/master/TimeTriggerCodeIO)
  * Code runs as slave to the [OpenDaVinci](https://github.com/se-research/OpenDaVINCI/) component `supercomponent`.
  * Is used for capturing data to understand Docker's impact on the input/output performance of the real-time application
