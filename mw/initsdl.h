#ifndef MW_INITSDL_H
#define MW_INITSDL_H

namespace mw {

class InitSdl {
public:
InitSdl();
virtual ~InitSdl();
private:
static int nbrOfInstances_;
};

} // namespace mw

#endif // MW_INITSDL_H