from setuptools import find_packages, setup

package_name = 'crimp_python'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Ethan Silver',
    maintainer_email='ethansilver248@gmail.com',
    description='The python package for the CRIMP robot',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'lidar = crimp_python.lidar:main'
        ],
    },
)
