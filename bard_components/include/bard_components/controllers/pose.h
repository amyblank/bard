#ifndef __BARD_COMPONENTS_CONTROLLERS_CARTESIAN_POSE_H
#define __BARD_COMPONENTS_CONTROLLERS_CARTESIAN_POSE_H

#include <iostream>

#include <boost/scoped_ptr.hpp>

#include <rtt/RTT.hpp>
#include <rtt/Port.hpp>

#include <kdl/jntarray.hpp>
#include <kdl/jntarrayvel.hpp>
#include <kdl/tree.hpp>
#include <kdl/frames.hpp>
#include <kdl/jacobian.hpp>
#include <kdl/chain.hpp>
#include <kdl/chainjnttojacsolver.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>

#include <tf/tf.h>

#include <geometry_msgs/TransformStamped.h>

namespace bard_components {
  namespace controllers {
    class CartesianPose : public RTT::TaskContext
    {
      // RTT Properties
      std::string robot_description_;
      std::string joint_prefix_;
      std::string root_link_;
      std::string tip_link_;
      std::string target_frame_;
      std::vector<double> Kp_; // Proportional gains                                                                                                                             
      std::vector<double> Kd_; // Derivative gains                                                                                                                               

      // RTT Ports
      RTT::InputPort<KDL::JntArrayVel> positions_in_port_;
      RTT::OutputPort<KDL::JntArray> torques_out_port_;

      RTT::OperationCaller<geometry_msgs::TransformStamped(const std::string&, const std::string&)> tf_lookup_transform_;

    public:
      CartesianPose(std::string const& name);
      bool configureHook();
      bool startHook();
      void updateHook();
      void stopHook();
      void cleanupHook();

    private:

      // Kinematic properties
      unsigned int n_dof_;
      KDL::Chain kdl_chain_;
      KDL::Tree kdl_tree_;
      urdf::Model urdf_model_;


      // Working variables
      KDL::JntArrayVel positions_;
      KDL::JntArrayVel positions_des_;
      KDL::JntArray torques_;

      // Joint limits
      KDL::JntArray joint_limits_min_;
      KDL::JntArray joint_limits_max_;

      // KDL IK solver which accounts for joint limits
      boost::scoped_ptr<KDL::ChainIkSolverPos> kdl_ik_solver_top_;
      boost::scoped_ptr<KDL::ChainIkSolverVel> kdl_ik_solver_vel_;

      // KDL FK solver
      boost::scoped_ptr<KDL::ChainFkSolverPos> kdl_fk_solver_pos_;
      boost::scoped_ptr<KDL::ChainJntToJacSolver> kdl_jacobian_solver_;

      geometry_msgs::TransformStamped tip_frame_msg_;
      tf::Transform tip_frame_tf_;
      KDL::Frame tip_frame_;
      KDL::Frame tip_frame_des_;

          
    };
  }
}


#endif // ifndef __BARD_COMPONENTS_CONTROLLERS_CARTESIAN_POSE_H

