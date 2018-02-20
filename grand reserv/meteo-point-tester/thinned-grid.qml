import Qt3D 2.0
import Qt3D.Renderer 2.0
import QtQuick 2.1
import QtQuick.Scene3D 2.0

Scene3D {
    id: scene3d
    anchors.fill: parent
    Entity {
        Camera {
            id: camera
            projectionType: CameraLens.PerspectiveProjection
            fieldOfView: 47
            aspectRatio: scene3d.width/scene3d.height
            nearPlane : 0.1
            farPlane : 1000.0
            position: Qt.vector3d( 7.0, 16.5, 17.0 )
            upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
            viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
        }

        Entity {
            components: [
                SphereMesh {
                    id : sphereMesh
                    rings: 50
                    slices: 100
                    shareable: false
                },

                Mesh {
                    id: myMesh
                    //source: "qrc:/test.obj"
                    source: "qrc:/lat-long-proc.obj"
                },

                PhongMaterial {}
            ]
        }

        components: [
            FrameGraph {
                activeFrameGraph: Viewport {
                    rect: Qt.rect(0.0, 0.0, 1.0, 1.0)
                    clearColor: "transparent"
                    CameraSelector {
                        camera: camera
                        ClearBuffer {
                            buffers : ClearBuffer.ColorDepthBuffer
                        }
                    }
                }
            }
        ]
    }
}
