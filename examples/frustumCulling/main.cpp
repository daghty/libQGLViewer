#include "frustumCulling.h"
#include "cullingCamera.h"
#include "box.h"
#include <qapplication.h>

using namespace qglviewer;

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  // Create octree AABBox hierarchy
  const qglviewer::Vec p(1.0, 0.7, 1.3);
  Box::Root = new Box(-p, p);
  Box::Root->buildBoxHierarchy(4);

  // Instantiate the two viewers.
  Viewer viewer, observer;

  // Give v a cullingCamera;
  Camera* c = viewer.camera();
  CullingCamera* cc = new CullingCamera();
  viewer.setCamera(cc);
  delete c;
  
  // Both viewers share the culling camera
  viewer.setCullingCamera(cc);
  observer.setCullingCamera(cc);

  // Place observer 
  observer.setSceneRadius(10.0);
  observer.camera()->setViewDirection(qglviewer::Vec(0.0, -1.0, 0.0));
  observer.showEntireScene();

  // Make sure every culling Camera movement updates the outer viewer
  QObject::connect(viewer.camera()->frame(), SIGNAL(manipulated()), &observer, SLOT(updateGL()));
  QObject::connect(viewer.camera()->frame(), SIGNAL(spun()), &observer, SLOT(updateGL()));


#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  application.setMainWidget(&viewer);
#else
  viewer.setWindowTitle("frustumCulling");
  observer.setWindowTitle("scene observer");
#endif

  // Show the viewers' windows.
  viewer.show();
  observer.show();

  return application.exec();
}
