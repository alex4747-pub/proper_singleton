# Proper Singleton

The Proper Singleton is a design pattern that resolves both the problems
of [the classic singleton](https://www.oreilly.com/library/view/design-patterns-elements/0201633612/)
and [the Meyer's singleton](https://vlsiuniverse.blogspot.com/2016/04/meyers-singleton-pattern.html)

Below is the summary of problem/features for known singleton variants
versus the proper-singleton (PS):
* Classic singleton with static initialization (CSSI)
* Classic singleton with lazy initialization (CSLI)
* Meyer's singleton with static initialization (MSSI)
* Meyer's singleton with lazy initialization (MSLI)

|Feature/Problem                       | CSSI | CSLI | MSSI | MSLI |PS   |
|:------------------------------------ |:----:|:----:|:----:|:----:|:---:|
|Static initialization problems        | Y    | N    | Y    | N    | N   |
|Requires lock during initialization   | N    | Y    | N    | N    | N   |
|Layered initialization support        | N    | Y    | N    | Y    | Y   |
|Abstract singleton support            | Y    | Y    | N    | N    | Y   |
|Run-time concrete singleton selection | N    | (*)  | N    | N    | Y   |
|Unit-test friendliness                | N    | N    | N    | N    | Y   |

(*) - it is possible but it will require creation of a custom singleton registry

The proper singleton has no problems and supports all the desired features
by starting from the classic singleton and utilizing properties of
[the simple_init_chain](https://github.com/alex4747-pub/simple_init_chain)
for initialization.

1. Initialization is performed from inside main function but before multi-threading
   is enabled. This way we avoid both static initialization order problems and
   necessity to lockout threads during initialization.

2. The layered initialization is supported by per-component initialization level.

3. Abstract singleton support and run-time concrete singleton selection are trivially
   implemented: each concrete singleton skips initialization if another concrete
   singleton is already installed, each concrete singleton makes the decision
   whether to instantiate self from either checking local environment or by reading
   config map.

   For example if we have a network line-card with two chipsets A and B with
   two drivers driver-A and driver-B implemented as singletons linked into
   the line-card image. The initialization routine for the driver-A checks presence
   of chipset A and instantiates self if the chipset is present. The driver-B
   behaves the same way. Both these drivers would no instantiate if another
   concrete driver is already instantiated to support unit testing.

4. The proper singleton is unit-test friendly:
* If production singletons are written
    in unit-test friendly manner (would not instantiate self if another concrete
    singleton is already instantiated) then all it takes to add mock singleton
    is to link it in with negative initialization level.
* Reset operation provides simple way to integrate mock daemons with setup/test/teardown
    pattern of traditional unit tests.
* Configuration map provides a convenient way to select a proper mock singleton for
    a specific test and to modify a mock singleton behavior on per-test basis.

# Example

## Base example

The example consists of two chipset drivers 'a' and 'b' implemented as singletons and
a network driver that uses instantiated singleton send packet.

A specific singleton is installed depending on the presence of the chipset hardware.
The chipset-b-driver supports reset operation.

The resets are globally disabled.

## Unit Test

It is expected that gtest environment is installed.

The resets are globally enabled.

The example/unit-test contains as simple unit test with mock-chipset-driver implemented
as a singleton with negative initialization level, so it will be instantiated instead of
real drivers.

The mock driver instance will constructed in SetUp and destructed inside TearDown.

